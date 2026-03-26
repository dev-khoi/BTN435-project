#include "camera.h"

camera::camera():isOn(true){}

std::string camera::handleCommand(const std::string& action){
    std::lock_guard<std::mutex> lock(mtx);

    if (action == "status") {
        return isOn ? "200 OK\nCamera is active"
                    : "200 OK\nCamera is inactive";
    }

    return "400 ERROR\nInvalid camera command";
}

std::string camera::handleCommand(
    const std::string& action,
    const std::string& value
){
    if (!value.empty()) {
        return "400 ERROR\nCamera command does not take a value";
    }

    std::lock_guard<std::mutex> lock(mtx);
    if (action == "status") {
        return isOn ? "200 OK\nCamera is active"
                    : "200 OK\nCamera is inactive";
    }
    return "400 ERROR\nInvalid camera command";
}

void camera::run(){
    
}
