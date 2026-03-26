#include "light.h"
#include <chrono>

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
    return Device::processRequest(requestLine);
}

void Light::run(){
    while (true){
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
