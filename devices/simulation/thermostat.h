#ifndef THERMOSTAT_H
#define THERMOSTAT_H
#include "device.h"

class thermostat : public Device {
    private:
        float temp;
        std::mutex mtx;
        std::string handleCommand(const std::string& action) override;
        std::string handleCommand(const std::string& action, const std::string& value) override;

    public:
        thermostat();
        void run() override;
};

#endif // THERMOSTAT_H