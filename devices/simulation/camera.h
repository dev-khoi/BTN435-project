#ifndef THERMOSTAT_H
#define THERMOSTAT_H
#include "device.h"

class camera : public Device {
    private:
        bool isOn;
        std::mutex mtx;
        std::string handleCommand(const std::string& action, const std::string& value) override;

    public:
        camera();
        void run() override;
};

#endif // THERMOSTAT_H