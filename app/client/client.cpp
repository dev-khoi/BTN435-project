#include "client.h"

#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

Client::Client(const std::string& ip, int port) 
    : clientSocket(-1), serverIP(ip), serverPort(port), connected(false){}

Client::~Client(){
    closeConnection();
}

bool Client::connectToServer() {
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        std::cerr << "Error: Could not create socket.\n";
        return false;
    }

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(serverPort);

    if(inet_pton(AF_INET, 
            serverIP.c_str(), 
            &serverAddress.sin_addr) 
            <= 0) {
        std::cerr << "Error: Invalid IP address.\n"
        close(clientSocket);
        clientSocket = -1;
        return false;
    }

    if(connect(clientSocket, 
            reinterpret_cast<sockaddr*>(&serverAddress), 
            sizeof(serverAddress)) < 0) {
        std::cerr << "Error: Connection to server failed.\n";
        close(clientSocket);
        clientSocket = -1;
        return false;
    }

    connected = true;
    std::cout << "Connected to server at " << serverIP << ":" << serverPort << "\n";
    return true;
}

bool Client::sendRequest(const std::string& request) {
    if(!connected) {
        std::cerr << "Error: Client is not connected to server.\n";
        return false;
    }

    std::string requestWithNewline = request + "\n";

    ssize_t bytesSent = send(clientSocket, 
        requestWithNewline.c_str(), 
        requestWithNewline.size(), 
        0);
    if (bytesSent < 0) {
        std::cerr << "Error: Failed to send request.\n";
        return false;
    }

    return true;
}

std::string Client::receiveResponse() {
    if (!connected) {
        return "Error: Not connected to server.";
    }

    char buffer[1024];
    std::memset(buffer, 0, sizeof(buffer));

    ssize_t bytesReceived = recv(clientSocket, 
        buffer, 
        sizeof(buffer) - 1, 
        0);
    if (bytesReceived < 0) {
        return "Error: Failed to receive response from server.";
    }

    if (bytesReceived == 0) {
        connected = false;
        return "Server dissconnected.";
    }

    return std::string(buffer);
}

void Client::closeConnection() {
    if (clientSocket >= 0){
        close(clientSocket);
        clientSocket = -1;
    }
    connected = false;
}

void Client::run() {
    if (!connectToServer()) {
        return;
    }

    std::string command;

    std::cout << "\nSmart Home Client\n";
    std::cout << "Enter commands like:\n";
    std::cout << "  GET /light/on\n";
    std::cout << "  GET /light/off\n";
    std::cout << "  GET /light/status\n";
    std::cout << "  GET /thermostat/set/22\n";
    std::cout << "  GET /thermostat/status\n";
    std::cout << "  GET /camera/status\n";
    std::cout << "Type 'exit' to quit.\n\n";

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, command);

        if (command.empty()){
            continue;
        }

        if (command == "exit" 
            || command == "EXIT" 
            || command == "Exit"){
            std::cout << "Closing client...\n"l
            break;
        }

        std::transform(command.begin(), command.end(), command.begin(),
                        [](unsigned char c){
                            return std::tolower(c);
                        });

        if (!sendRequest(command)) {
            std::cerr << "Request could not be sent.\n";
            break;
        }

        std::string response = receiveResponse();
        std::cout << "Server response:\n" << response << "\n";

        if (!connected) {
            break;
        }
    }
    closeConnection();
}

int main(int argc, char* argv[]) {
    std::string ip = "127.0.0.1";
    int port = 8080;

    if (argv >= 3) {
        port = std::stoi(argv[2]);
    }

    Client client(ip, port);
    client.run();

    return 0;
}