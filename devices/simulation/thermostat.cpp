#include "thermostat.h"

thermostat::thermostat():temp(21.5){}

std::string thermostat::handleCommand(const std::string& action){
    std::lock_guard<std::mutex> lock(mtx);

    if (action == "status") {
        return "200 OK\nthermostat temperature is " + std::to_string(temp);
    }

    return "400 ERROR\nInvalid thermostat command";
}

std::string thermostat::handleCommand(
    const std::string& action,
    const std::string& value
){
    std::lock_guard<std::mutex> lock(mtx);

    if(action == "set"){
        if (value.empty()) {
            return "400 ERROR\nMissing thermostat value";
        }
        temp = std::stof(value);
        return "200 OK\nthermostat set to " + value;
    }

    return "400 ERROR\nInvalid thermostat command";
}

void thermostat::run(){
    
}
