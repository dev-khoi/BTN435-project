#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib") // auto-link Winsock on MSVC
using ssize_t = SSIZE_T;
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#endif
#include <cstddef>
#include "server.h"
#include <iostream>
#include <sstream>
#include <cstring>
namespace
{
    constexpr int BACKLOG = 16;
    constexpr std::size_t BUFFER_SIZE = 1024;
}

Server::Server(int port)
    : serverSocket(-1),
      port(port),
      running(false) {}

Server::~Server()
{
    stop();
}

bool Server::start()
{
    if (running.load())
    {
        return true;
    }

    try
    {
#ifdef _WIN32
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        {
            throw std::runtime_error("WSAStartup failed");
        }
#endif

        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket < 0)
        {
            throw std::runtime_error("Socket creation failed");
        }

        int opt = 1;
        setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char *>(&opt), sizeof(opt));

        sockaddr_in serverAddress{};
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = INADDR_ANY;
        serverAddress.sin_port = htons(static_cast<uint16_t>(port));

        if (bind(serverSocket, reinterpret_cast<sockaddr *>(&serverAddress), sizeof(serverAddress)) < 0)
        {
            throw std::runtime_error("Bind failed");
        }

        if (listen(serverSocket, BACKLOG) < 0)
        {
            throw std::runtime_error("Listen failed");
        }

        running.store(true);
        startWorkers();

        std::cout << "Server listening on port " << port << "\n";
        acceptLoop();
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Server start error: " << e.what() << "\n";
        if (serverSocket >= 0)
        {
            CLOSE_SOCKET(serverSocket);
            serverSocket = -1;
        }
        return false;
    }
}

void Server::stop()
{
    bool wasRunning = running.exchange(false);
    if (!wasRunning && serverSocket < 0)
    {
        return;
    }

    if (serverSocket >= 0)
    {
        shutdown(serverSocket, SHUT_RDWR);
        CLOSE_SOCKET(serverSocket);
        serverSocket = -1;
    }

    stopWorkers();

    std::lock_guard<std::mutex> lock(clientThreadsMutex);
    for (std::thread &t : clientThreads)
    {
        if (t.joinable())
        {
            t.join();
        }
    }
    clientThreads.clear();
}

void Server::acceptLoop()
{
    while (running.load())
    {
        sockaddr_in clientAddress{};
        socklen_t clientLen = sizeof(clientAddress);

        int clientSocket = accept(serverSocket, reinterpret_cast<sockaddr *>(&clientAddress), &clientLen);
        if (clientSocket < 0)
        {
            if (!running.load())
            {
                break;
            }
            continue;
        }

        std::lock_guard<std::mutex> lock(clientThreadsMutex);
        clientThreads.emplace_back(&Server::handleClient, this, clientSocket);
    }
}

void Server::handleClient(int clientSocket)
{
    char buffer[BUFFER_SIZE];
    std::string pending;

    while (running.load())
    {
        std::memset(buffer, 0, sizeof(buffer));
        ssize_t n = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (n <= 0)
        {
            break;
        }

        pending.append(buffer, static_cast<std::size_t>(n));

        std::size_t pos = std::string::npos;
        while ((pos = pending.find('\n')) != std::string::npos)
        {
            std::string request = pending.substr(0, pos);
            pending.erase(0, pos + 1);

            request = trim(request);
            if (request.empty())
            {
                continue;
            }

            std::string response = processRequest(request);
            if (!sendAll(clientSocket, response))
            {
                break;
            }
        }
    }

    CLOSE_SOCKET(clientSocket);
}

std::string Server::processRequest(const std::string &requestLine)
{
    std::istringstream iss(requestLine);
    std::string method;
    std::string path;
    std::string extra;

    iss >> method >> path;
    if (method.empty() || path.empty() || (iss >> extra))
    {
        return makeResponse(400, "ERROR", "Invalid command format");
    }

    method = toLower(method);
    if (method != "get")
    {
        return makeResponse(400, "ERROR", "Only GET is supported");
    }

    if (path.empty() || path.front() != '/')
    {
        return makeResponse(400, "ERROR", "Invalid path");
    }

    std::vector<std::string> parts = split(path.substr(1), '/');
    if (parts.size() < 2 || parts.size() > 3)
    {
        return makeResponse(400, "ERROR", "Invalid command format");
    }

    std::string device = toLower(parts[0]);
    std::string action = toLower(parts[1]);
    bool hasValue = (parts.size() == 3);
    int value = 0;

    if (hasValue)
    {
        try
        {
            value = std::stoi(parts[2]);
        }
        catch (...)
        {
            return makeResponse(400, "ERROR", "Invalid numeric value");
        }
    }

    if (device == "light")
    {
        return dispatchLight(action, hasValue, value);
    }
    if (device == "thermostat")
    {
        return dispatchThermostat(action, hasValue, value);
    }
    if (device == "camera")
    {
        return dispatchCamera(action, hasValue, value);
    }
    if (device == "network")
    {
        return dispatchNetwork(action, hasValue);
    }

    return makeResponse(404, "NOT FOUND", "Unknown device");
}

std::string Server::dispatchLight(const std::string &action, bool hasValue, int value)
{
    DeviceTask task{action, value, hasValue, std::promise<std::string>()};
    std::future<std::string> result = task.promise.get_future();

    {
        std::lock_guard<std::mutex> lock(lightQueueMutex);
        lightQueue.push(std::move(task));
    }
    lightCv.notify_one();

    return result.get();
}

std::string Server::dispatchThermostat(const std::string &action, bool hasValue, int value)
{
    DeviceTask task{action, value, hasValue, std::promise<std::string>()};
    std::future<std::string> result = task.promise.get_future();

    {
        std::lock_guard<std::mutex> lock(thermostatQueueMutex);
        thermostatQueue.push(std::move(task));
    }
    thermostatCv.notify_one();

    return result.get();
}

std::string Server::dispatchCamera(const std::string &action, bool hasValue, int value)
{
    DeviceTask task{action, value, hasValue, std::promise<std::string>()};
    std::future<std::string> result = task.promise.get_future();

    {
        std::lock_guard<std::mutex> lock(cameraQueueMutex);
        cameraQueue.push(std::move(task));
    }
    cameraCv.notify_one();

    return result.get();
}

std::string Server::dispatchNetwork(const std::string &action, bool hasValue)
{
    if (hasValue)
    {
        return makeResponse(400, "ERROR", "Network command does not take a value");
    }

    if (action == "arp")
    {
        return makeResponse(200, "OK", arpHandler.formatTable());
    }
    if (action == "routing")
    {
        return makeResponse(200, "OK", routingHandler.formatTable());
    }

    return makeResponse(404, "NOT FOUND", "Unknown network resource");
}

void Server::lightWorker()
{
    while (running.load() || !lightQueue.empty())
    {
        DeviceTask task;

        {
            std::unique_lock<std::mutex> lock(lightQueueMutex);
            lightCv.wait(lock, [this]
                         { return !running.load() || !lightQueue.empty(); });
            if (lightQueue.empty())
            {
                continue;
            }
            task = std::move(lightQueue.front());
            lightQueue.pop();
        }

        task.promise.set_value(execLight(task));
    }
}

void Server::thermostatWorker()
{
    while (running.load() || !thermostatQueue.empty())
    {
        DeviceTask task;

        {
            std::unique_lock<std::mutex> lock(thermostatQueueMutex);
            thermostatCv.wait(lock, [this]
                              { return !running.load() || !thermostatQueue.empty(); });
            if (thermostatQueue.empty())
            {
                continue;
            }
            task = std::move(thermostatQueue.front());
            thermostatQueue.pop();
        }

        task.promise.set_value(execThermostat(task));
    }
}

void Server::cameraWorker()
{
    while (running.load() || !cameraQueue.empty())
    {
        DeviceTask task;

        {
            std::unique_lock<std::mutex> lock(cameraQueueMutex);
            cameraCv.wait(lock, [this]
                          { return !running.load() || !cameraQueue.empty(); });
            if (cameraQueue.empty())
            {
                continue;
            }
            task = std::move(cameraQueue.front());
            cameraQueue.pop();
        }

        task.promise.set_value(execCamera(task));
    }
}

std::string Server::execLight(const DeviceTask &task)
{
    std::string request = "GET /light/" + task.action;
    if (task.hasValue)
    {
        request += "/" + std::to_string(task.value);
    }
    return lightDevice.processRequest(request);
}

std::string Server::execThermostat(const DeviceTask &task)
{
    std::string request = "GET /thermostat/" + task.action;
    if (task.hasValue)
    {
        request += "/" + std::to_string(task.value);
    }
    return thermostatDevice.processRequest(request);
}

std::string Server::execCamera(const DeviceTask &task)
{
    std::string request = "GET /camera/" + task.action;
    if (task.hasValue)
    {
        request += "/" + std::to_string(task.value);
    }
    return cameraDevice.processRequest(request);
}

void Server::startWorkers()
{
    lightThread = std::thread(&Server::lightWorker, this);
    thermostatThread = std::thread(&Server::thermostatWorker, this);
    cameraThread = std::thread(&Server::cameraWorker, this);
}

void Server::stopWorkers()
{
    lightCv.notify_all();
    thermostatCv.notify_all();
    cameraCv.notify_all();

    if (lightThread.joinable())
    {
        lightThread.join();
    }
    if (thermostatThread.joinable())
    {
        thermostatThread.join();
    }
    if (cameraThread.joinable())
    {
        cameraThread.join();
    }
}

std::string Server::toLower(std::string s)
{
    for (char &c : s)
    {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    return s;
}

std::string Server::trim(const std::string &s)
{
    std::size_t start = 0;
    while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start])) != 0)
    {
        ++start;
    }

    if (start == s.size())
    {
        return "";
    }

    std::size_t end = s.size() - 1;
    while (end > start && std::isspace(static_cast<unsigned char>(s[end])) != 0)
    {
        --end;
    }

    return s.substr(start, end - start + 1);
}

std::vector<std::string> Server::split(const std::string &s, char delimiter)
{
    std::vector<std::string> out;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delimiter))
    {
        if (!item.empty())
        {
            out.push_back(item);
        }
    }
    return out;
}

bool Server::sendAll(int socketFd, const std::string &data)
{
    std::size_t sentTotal = 0;
    while (sentTotal < data.size())
    {
        ssize_t sent = send(socketFd, data.c_str() + sentTotal, data.size() - sentTotal, 0);
        if (sent <= 0)
        {
            return false;
        }
        sentTotal += static_cast<std::size_t>(sent);
    }
    return true;
}

std::string Server::makeResponse(int code, const std::string &status, const std::string &body)
{
    return std::to_string(code) + " " + status + "\n" + body + "\n";
}

int main(int argc, char *argv[])
{
    int port = 8080;

    if (argc > 1)
    {
        port = std::stoi(argv[1]);
    }

    Server server(port);
    if (!server.start())
    {
        return 1;
    }

    return 0;
}
