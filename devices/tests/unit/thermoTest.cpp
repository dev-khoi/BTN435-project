#include "baseTest.h"

int main(){

    std::cout << " ===== THERMOSTAT TESTS  ===== \n";
    Thermostat t;

    TEST("default status show 21,5",
        t.processRequest("GET /thermostat/status").find("21.5") != std::string::npos);
    
    TEST("set to 25 returns 200",
        t.processRequest("GET /thermostat/set/25").find("200") != std::string::npos);

    TEST("status after set to 25 shows 25",
        t.processRequest("GET /thermostat/status").find("25") != std::string::npos);
    
    TEST("invalid set value returns 400",
        t.processRequest("GET /thermostat/set/abc").find("400") != std::string::npos);

    TEST("unknown command returns 400",
        t.processRequest("GET /thermostat/reset").find("400") != std::string::npos);

    std::cout << std::endl << passed << " tests passed, " << failed << " tests failed.\n";
    return failed > 0 ? 1 : 0;
}