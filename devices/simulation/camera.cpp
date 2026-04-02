#include "camera.h"

Camera::Camera():isOn(true){}

std::string Camera::handleCommand(const std::string& action){
    std::lock_guard<std::mutex> lock(mtx);

    if (action == "status") {
        return isOn ? makeResponse(200, "OK", "Camera is ONLINE")
                    : makeResponse(200, "OK", "Camera is OFFLINE");
    }

    return makeResponse(400, "ERROR", "Invalid camera command");
}

std::string Camera::handleCommand(
    const std::string& action,
    const std::string& value
){
    if (!value.empty()) {
        return makeResponse(400, "ERROR", "Camera command does not take a value");
    }

    std::lock_guard<std::mutex> lock(mtx);
    if (action == "status") {
        return isOn ? makeResponse(200, "OK", "Camera is ONLINE")
                    : makeResponse(200, "OK", "Camera is OFFLINE");
    }
    return makeResponse(400, "ERROR", "Invalid camera command");
}

void Camera::run(){
    
}
