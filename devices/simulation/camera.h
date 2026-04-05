#ifndef CAMERA_H
#define CAMERA_H
#include "device.h"

class Camera : public Device {
    private:
        bool isOn;
        std::mutex mtx;
        std::string handleCommand(const std::string& action) override;
        std::string handleCommand(const std::string& action, const std::string& value) override;

    public:
        std::atomic<bool> stopRequested; 
        Camera();
        void run() override;
};

#endif // CAMERA_H
