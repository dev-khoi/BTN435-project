#include "light.h"

Light::Light() : isOn(false), stopRequested(false) {}

// "GET /light/<action>/<value>"
std::string Light::handleCommand(const std::string& action,
                                 const std::string& value) 
{
    return makeResponse(400, "ERROR", "Light command does not take a value");
}

std::string Light::handleCommand(const std::string& action) {
    // Device-local mutex protects light state reads and updates.
    std::lock_guard<std::mutex> lock(mtx);

    if (action == "on") {
        isOn = true;
        return makeResponse(200, "OK", "Light turned ON");
    }
    if (action == "off") {
        isOn = false;
        return makeResponse(200, "OK", "Light turned OFF");
    }
    if (action == "status") {
        return isOn ? makeResponse(200, "OK", "Light is ON")
                    : makeResponse(200, "OK", "Light is OFF");
    }

    return makeResponse(400, "ERROR", "Invalid light command");
}

void Light::run() {
    while (!stopRequested.load()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
