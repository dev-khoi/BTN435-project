#include "light.h"
#include <chrono>

Light::Light() : isOn(false) {}

std::string Light::handleCommand(const std::string& action){
    // Device-local mutex protects shared light state for concurrent requests.
    std::lock_guard<std::mutex> lock(mtx);

    if(action == "on"){
        isOn = true;
        return makeResponse(200, "OK", "Light turned ON");
    }
    else if (action == "off"){
        isOn = false;
        return makeResponse(200, "OK", "Light turned OFF");
    }
    else if (action == "status"){
        return isOn ? makeResponse(200, "OK", "Light is ON")
                    : makeResponse(200, "OK", "Light is OFF");
    }

    return makeResponse(400, "ERROR", "Invalid light command");
}

std::string Light::handleCommand(const std::string& action, const std::string& value){
    if (!value.empty()) {
        return makeResponse(400, "ERROR", "Light command does not take a value");
    }
    return handleCommand(action);
}

std::string Light::processRequest(const std::string &requestLine){
    return Device::processRequest(requestLine);
}

void Light::run(){
    while (true){
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
