#ifndef LIGHT_H
#define LIGHT_H
#include "device.h"

class Light : public Device {
    private:
        bool isOn;
        std::mutex mtx;
        std::string handleCommand(const std::string& action) override;
    public:
        Light();
        virtual std::string processRequest(const std::string &requestLine) override;
        void run() override;
};

#endif // LIGHT_H