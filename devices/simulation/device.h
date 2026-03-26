#ifndef DEVICE_H
#define DEVICE_H

#include "utility.h"

#include <string>
#include <mutex>
#include <future>
#include <thread>


// i want token to auntenication
inline Utils ul;

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
        virtual std::string handleCommand(const std::string& action) = 0;
    
    protected:
        // makeResponse
        std::string makeResponse(int code, const std::string &status, const std::string &body){
            return std::to_string(code) + " " + status + "\n" + body + "\n";
        }
    public:
        virtual std::string processRequest(const std::string &requestLine) {
            std::istringstream iss(requestLine);
            std::string method;
            std::string path;
            std::string extra;

            iss >> method >> path;
            if (method.empty() || path.empty() || (iss >> extra)) {
                return makeResponse(400, "ERROR", "Invalid command format");
            }

            method = ul.toLower(method);
            if (method != "get") {
                return makeResponse(400, "ERROR", "Only GET is supported");
            }

            if (path.empty() || path.front() != '/') {
                return makeResponse(400, "ERROR", "Invalid path");
            }

            std::vector<std::string> parts = ul.split(path.substr(1), '/');
            if (parts.size() != 2 && parts.size() != 3) {
                return makeResponse(400, "ERROR", "Invalid command format");
            }

            const std::string action = ul.toLower(parts[1]);
            if (parts.size() == 3) {
                return handleCommand(action, parts[2]);
            }

            return handleCommand(action);
        }
        virtual void run() = 0;
        virtual ~Device() {};
};

#endif // DEVICE_H
