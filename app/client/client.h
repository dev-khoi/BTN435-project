#ifndef CLIENT_H
#define CLIENT_H

#include <string>

class Client {
private: 
    int clientSocket;
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