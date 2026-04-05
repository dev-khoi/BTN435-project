#include "camera.h"

Camera::Camera():isOn(true), stopRequested(false){}

std::string Camera::handleCommand(const std::string& action){
    // Device-local mutex protects camera state reads.
    std::lock_guard<std::mutex> lock(mtx);

    if (action == "status") {
        return isOn ? makeResponse(200, "OK", "Camera is ONLINE")
                    : makeResponse(200, "OK", "Camera is OFFLINE");
    }

    if (action == "on"){
        isOn = true;
        return makeResponse(200, "OK", "Camera turned ON");
    }

    if (action == "off"){
        isOn = false;
        return makeResponse(200, "OK", "Camera turned OFF");
    }

    return makeResponse(400, "ERROR", "Invalid camera command");
}

std::string Camera::handleCommand(
    const std::string& action,
    const std::string& value
){
    return makeResponse(400, "ERROR", "Invalid camera command");
}

void Camera::run(){
    while (!stopRequested.load()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
