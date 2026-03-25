#ifndef DEVICE_H
#define DEVICE_H

#include "utility.h"

#include <string>
#include <mutex>
#include <future>
#include <thread>


// i want token to auntenication
Utils ul;

class Device {
    private:
        struct DeviceTask
            {
                std::string action;
                int value;
                bool hasValue;
                std::promise<std::string> promise;
            };

        virtual std::string handleCommand(const std::string& action, const std::string& value) = 0;
        virtual std::string handleCmand(const std::string& action) = 0;
    
    protected:
        // makeResponse
        std::string makeResponse(int code, const std::string &status, const std::string &body){
            return std::to_string(code) + " " + status + "\n" + body + "\n";
        }
    public:
        virtual std::string processRequest(const std::string &requestLine);
        virtual void run() = 0;
        virtual ~Device() {};
};

#endif // DEVICE_H