cd BTN435-project\devices\tests\builds
cd BTN435-project/devices/tests/builds

# From devices/tests/builds/

g++ -std=c++17 -pthread -I ../../simulation ../../simulation/light.cpp ../../simulation/thermostat.cpp ../../simulation/camera.cpp ../unit/lightTest.cpp -o lightTest
./lightTest

g++ -std=c++17 -pthread -I ../../simulation ../../simulation/light.cpp ../../simulation/thermostat.cpp ../../simulation/camera.cpp ../unit/thermoTest.cpp -o thermoTest
./thermoTest

g++ -std=c++17 -pthread -I ../../simulation ../../simulation/light.cpp ../../simulation/thermostat.cpp ../../simulation/camera.cpp ../unit/cameraTest.cpp -o cameraTest
./cameraTest

g++ -std=c++17 -pthread -I ../../simulation ../../simulation/light.cpp ../../simulation/thermostat.cpp ../../simulation/camera.cpp ../integration/integration.cpp -o integrationTest
./integrationTest
