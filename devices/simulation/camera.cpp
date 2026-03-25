#include "camera.h"

camera::camera():isOn(true){}
std::string camera::handleCommand(
    const std::string& action,
    const std::string& value
){
    std::lock_guard<std::mutex> lock(mtx);
    if (action == "status" ){
        if (isOn) return "200 OK\nCamera is active"
    }
    return "400 ERROR\nInvalid Camera command";
}

void camera::run(){
    
}