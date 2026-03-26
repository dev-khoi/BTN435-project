#ifndef CAMERA_H
#define CAMERA_H
#include "device.h"

class camera : public Device {
    private:
        bool isOn;
        std::mutex mtx;
        std::string handleCommand(const std::string& action) override;
        std::string handleCommand(const std::string& action, const std::string& value) override;

    public:
        camera();
        void run() override;
};

#endif // CAMERA_H
