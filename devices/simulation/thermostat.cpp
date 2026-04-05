#include "thermostat.h"

Thermostat::Thermostat():temp(21.5f), stopRequested(false) {}

std::string Thermostat::handleCommand(const std::string& action){
    // Device-local mutex protects temperature reads.
    std::lock_guard<std::mutex> lock(mtx);

    if (action == "status") {
        return makeResponse(200, "OK", "Thermostat temperature is " + std::to_string(temp) + "°C");
    }

    return makeResponse(400, "ERROR", "Invalid thermostat command");
}

std::string Thermostat::handleCommand(
    const std::string& action,
    const std::string& value
){
    if(action == "set"){
        if (value.empty()) {
            return makeResponse(400, "ERROR", "Missing thermostat value");
        }
        float newTemp;
        try {
            std::size_t pos;
            newTemp = std::stof(value, &pos);
            if (pos != value.size()) {
                return makeResponse(400, "ERROR", "Invalid thermostat value");
            }
        } catch (const std::exception&) {
            return makeResponse(400, "ERROR", "Invalid thermostat value");
        }
        // Device-local mutex protects temperature updates.
        std::lock_guard<std::mutex> lock(mtx);
        temp = newTemp;
        return makeResponse(200, "OK", "Thermostat temperature set to " + std::to_string(temp) + "°C");
    }
    return makeResponse(400, "ERROR", "Invalid thermostat command");
}

void Thermostat::run(){
    while(!stopRequested.load()){
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
