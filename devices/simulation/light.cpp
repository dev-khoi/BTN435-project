#include "light.h"

Light::Light() : isOn(false) {}

std::string Light::handleCommand(const std::string& action){
    
    std::lock_guard<std::mutex> lock(mtx);

    if(action == "on"){
        isOn = true;
        return "200 OK\nLight turned ON";
    }
    else if (action == "off"){
        isOn = false;
        return "200 OK\nLight turned OFF";
    }
    else if (action == "status"){
        return isOn ? "200 OK\nLight is ON"
                    : "200 OK\nLight turned OFF";
    }

    return "400 ERROR\nInvalid light command";
}

std::string Light::processRequest(const std::string &requestLine){
    std::istringstream iss(requestLine);
    std::string method;
    std::string path;
    std::string extra;

    iss >> method >> path;
    if (method.empty() || path.empty() || (iss >> extra))
        return makeResponse(400, "ERROR", "Invalid command format");
    

    method = ul.toLower(method);
    if (method != "get")return makeResponse(400, "ERROR", "Only GET is supported");
    if (path.empty() || path.front() != '/') return makeResponse(400, "ERROR", "Invalid path");


    std::vector<std::string> parts = ul.split(path.substr(1), '/');
    if (parts.size() == 2 )return makeResponse(400, "ERROR", "Invalid command format");

    std::string action = ul.toLower(parts[1]);

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

    // return makeResponse(404, "NOT FOUND", "Unknown device");
}

void Light::run(){
    while (true){
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}