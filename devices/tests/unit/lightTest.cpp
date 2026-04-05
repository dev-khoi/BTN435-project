#include "baseTest.h"

int main() {
    std::cout << " ===== LIGHT TESTS  ===== \n";
    Light light;

    TEST("turn on returns 200",
        light.processRequest("GET /light/on").find("200") != std::string::npos);

    TEST("turn on set state to ON",
        light.processRequest("GET /light/status").find("ON") != std::string::npos);
    
    TEST("turn off returns 200",
        light.processRequest("GET /light/off").find("200") != std::string::npos);
    
    TEST("turn off set state to OFF",
        light.processRequest("GET /light/status").find("OFF") != std::string::npos);

    TEST("unknown command returns 400",
        light.processRequest("GET /light/blink").find("400") != std::string::npos);

    TEST("command with value returns 400",
        light.processRequest("GET /light/set/50").find("400") != std::string::npos);

    std::cout << std::endl << passed << " tests passed, " << failed << " tests failed.\n";
    return failed > 0 ? 1 : 0;
}