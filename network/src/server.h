#ifndef SERVER_H
#define SERVER_H

#include <atomic>
#include <condition_variable>
#include <future>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

#include "arp_handler.h"

#ifdef _WIN32
#define CLOSE_SOCKET(s) closesocket(s)
#define SHUT_RDWR SD_BOTH
#else
#define CLOSE_SOCKET(s) close(s)
#endif
// ~SOME NOTES ON THE WORK (CAN BE REMOVED LATER)
// std::condition_variable: Condition variable to synchronize threads
// device task structure
struct DeviceTask
{
    std::string action;
    int value;
    bool hasValue;
    std::promise<std::string> promise;
};
// server class
class Server
{
public:
    Server(int port = 8080);
    ~Server();

    bool start();
    void stop();

private:
    int serverSocket;
    int port;
    std::atomic<bool> running;

    // shared device states are protected by a mutex
    bool lightOn;
    int thermostatTemperature;
    bool cameraOnline;
    std::mutex stateMutex;
    
    std::vector<std::thread> clientThreads;
    std::mutex clientThreadsMutex;

    std::queue<DeviceTask> lightQueue;
    std::queue<DeviceTask> thermostatQueue;
    std::queue<DeviceTask> cameraQueue;

    std::mutex lightQueueMutex;
    std::mutex thermostatQueueMutex;
    std::mutex cameraQueueMutex;

    std::condition_variable lightCv;
    std::condition_variable thermostatCv;
    std::condition_variable cameraCv;

    std::thread lightThread;
    std::thread thermostatThread;
    std::thread cameraThread;

    ArpHandler arpHandler;

    void acceptLoop();
    void handleClient(int clientSocket);

    std::string processRequest(const std::string &requestLine);

    std::string dispatchLight(const std::string &action, bool hasValue, int value);
    std::string dispatchThermostat(const std::string &action, bool hasValue, int value);
    std::string dispatchCamera(const std::string &action, bool hasValue, int value);
    std::string dispatchNetwork(const std::string &action, bool hasValue);

    void lightWorker();
    void thermostatWorker();
    void cameraWorker();

    std::string execLight(const DeviceTask &task);
    std::string execThermostat(const DeviceTask &task);
    std::string execCamera(const DeviceTask &task);

    void startWorkers();
    void stopWorkers();

    static std::string toLower(std::string s);
    static std::string trim(const std::string &s);
    static std::vector<std::string> split(const std::string &s, char delimiter);
    static bool sendAll(int socketFd, const std::string &data);
    static std::string makeResponse(int code, const std::string &status, const std::string &body);
};

#endif
