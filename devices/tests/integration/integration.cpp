#include <iostream>
#include <cassert>
#include "../../simulation/devices.h"
 
int passed = 0;
int failed = 0;
 
#define TEST(name, expr) \
    do { \
        if (expr) { std::cout << "  [PASS] " << name << "\n"; passed++; } \
        else      { std::cout << "  [FAIL] " << name << "\n"; failed++; } \
    } while(0)

int main() {
    std::cout << " ===== INTEGRATION TESTS  ===== \n";

    // --- Test 1: single client controls all three devices ---
    {
        std::cout << "\n[1] Single client controls all devices\n";
        Light light;
        Thermostat thermo;
        Camera camera;

        TEST("Light on returns 200",
            light.processRequest("GET /light/on").find("200") != std::string::npos);
        TEST("Thermostat set to 22 returns 200",
            thermo.processRequest("GET /thermostat/set/22").find("200") != std::string::npos);
        TEST("Camera start returns 200",
            camera.processRequest("GET /camera/status").find("200") != std::string::npos);
        
        TEST("Light status shows ON",
            light.processRequest("GET /light/status").find("ON") != std::string::npos);
        TEST("Thermostat status shows 22",
            thermo.processRequest("GET /thermostat/status").find("22") != std::string::npos);
        TEST("Camera status shows ONLINE",
            camera.processRequest("GET /camera/status").find("ONLINE") != std::string::npos);
    }

    // --- Test 2:  multiple clients simultaneously ---
    {
        std::cout << "\n[2] Multiple clients simultaneously\n";
        Light light;
        Thermostat thermo;
        Camera cam;

        std::thread clientA([&](){
            for (int i = 0; i < 20; i++)
                light.processRequest(i % 2 == 0 ? "GET /light/on" : "GET /light/off");
        });

        std::thread clientB([&]() {
            for (int i = 0; i < 20; i++)
                thermo.processRequest("GET /thermostat/set/" + std::to_string(20 + i % 5));
        });

        std::thread clientC([&]() {
            for (int i = 0; i < 20; i++)
                cam.processRequest("GET /camera/status");
        });

        clientA.join();
        clientB.join();
        clientC.join();

        TEST("light still responds after concurrent access",
            light.processRequest("GET /light/status").find("200") != std::string::npos);
        TEST("thermostat still responds after concurrent access",
            thermo.processRequest("GET /thermostat/status").find("200") != std::string::npos);
        TEST("camera still responds after concurrent access",
            cam.processRequest("GET /camera/status").find("200") != std::string::npos);

    }

    // --- Test 3: invalid commands ---
    {
        std::cout << "\n[3] Invalid commands\n";
        Light light;
        Thermostat thermo;
        Camera cam;

        TEST("bad light command returns 400",      light.processRequest("GET /light/explode").find("400")        != std::string::npos);
        TEST("bad thermostat value returns 400",   thermo.processRequest("GET /thermostat/set/abc").find("400")  != std::string::npos);
        TEST("bad camera command returns 400",     cam.processRequest("GET /camera/record").find("400")          != std::string::npos);

        TEST("light still works after bad command",     light.processRequest("GET /light/on").find("200")            != std::string::npos);
        TEST("thermostat still works after bad command",thermo.processRequest("GET /thermostat/set/22").find("200")   != std::string::npos);
        TEST("camera still works after bad command",    cam.processRequest("GET /camera/status").find("200")          != std::string::npos);
    }

    std::cout << "\n============================\n";
    std::cout << std::endl << passed << " tests passed, " << failed << " tests failed.\n";
    return failed > 0 ? 1 : 0;
}