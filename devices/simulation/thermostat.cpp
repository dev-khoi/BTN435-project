#include "thermostat.h"

Thermostat::Thermostat():temp(21.5f){}

std::string Thermostat::handleCommand(const std::string& action){
    // Device-local mutex protects temperature reads.
    std::lock_guard<std::mutex> lock(mtx);

    if (action == "status") {
        return makeResponse(200, "OK", "Thermostat temperature is " + std::to_string(temp));
    }

    return makeResponse(400, "ERROR", "Invalid thermostat command");
}

std::string Thermostat::handleCommand(
    const std::string& action,
    const std::string& value
){
    // Device-local mutex protects temperature updates.
    std::lock_guard<std::mutex> lock(mtx);

    if(action == "set"){
        if (value.empty()) {
            return makeResponse(400, "ERROR", "Missing thermostat value");
        }
        try {
            temp = std::stof(value);
        } catch (...) {
            return makeResponse(400, "ERROR", "Invalid thermostat value");
        }
        return makeResponse(200, "OK", "Thermostat set to " + value);
    }

    return makeResponse(400, "ERROR", "Invalid thermostat command");
}

void Thermostat::run(){
    
}
