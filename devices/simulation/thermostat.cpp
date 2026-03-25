#include "thermostat.h"

thermostat::thermostat():temp(21.5){}
std::string thermostat::handleCommand(
    const std::string& action,
    const std::string& value
){
    if (!value) return "error";
    std::lock_guard<std::mutex> lock(mtx);
    if(action == "set"){
        temp = std::stof(value);
        return "200 OK\n thermastat set to " + value;
    }else if(action == "status"){
        return "200 OK\n thermastat temperature is " + std::to_string(temp);
    }

    return "400 ERROR\nInvalid thermastat command";
}

void thermostat::run(){
    
}