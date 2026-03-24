#ifndef CLIENT_H
#define CLIENT_H

#include <string>

#ifdef _WIN32
#include <winsock2.h>
using ClientSocket = SOCKET;
#else
using ClientSocket = int;
#endif

class Client {
private: 
    ClientSocket clientSocket;
    std::string serverIP;
    int serverPort;
    bool connected;

public:
    Client(const std::string& ip = "127.0.0.1", int port = 8080);
    ~Client();

    bool connectToServer();
    bool sendRequest(const std::string& request);
    std::string receiveResponse();
    void run();
    void closeConnection();
};

#endif
