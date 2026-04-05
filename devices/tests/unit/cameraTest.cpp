#include "baseTest.h"

int main(){
    std::cout << " ===== CAMERA TESTS  ===== \n";
    Camera c;
    
    TEST("start active",
        c.processRequest("GET /camera/status").find("ONLINE") != std::string::npos);
    
    TEST("turn off returns 200",
        c.processRequest("GET /camera/off").find("200") != std::string::npos);

    TEST("status shows inactive (after off)",
        c.processRequest("GET /camera/status").find("OFFLINE") != std::string::npos);
    
    TEST("turn on returns 200",
        c.processRequest("GET /camera/on").find("200") != std::string::npos);
    
    TEST("status shows active after on",
        c.processRequest("GET /camera/status").find("ONLINE") != std::string::npos);

    TEST("unknown command returns 400",
        c.processRequest("GET /camera/zoom").find("400") != std::string::npos);        
    
    TEST("command with value returns 400",
        c.processRequest("GET /camera/set/50").find("400") != std::string::npos);

    std::cout << std::endl << passed << " tests passed, " << failed << " tests failed.\n";
    return failed > 0 ? 1 : 0;
}