#include "controller_serialtestserver.h"
#include "util.h"

network_mode_t network_mode;

Controller::Controller()
{
    network = LGNetwork();
}

void Controller::setup()
{
	LGSerial::init(); // (9830400/(16*9600))-1;

    network.set_mode(LGNETWORK_DISCOVER);
    // LGSerial::print("Ready");

    unsigned long long start_time = millis();

    while(millis() < (start_time + 10000)) { // 10 second associate time
        network.loop();
    }

}

void Controller::loop()
{
    LGSerial::put('F');
    sleep(1000);
    return;
    network.set_mode(LGNETWORK_OPERATE);
    // if(millis() % 1000 == 0) {

    // }

    // network.loop();
    // LGSerial::put("Loop");
    // char c = LGSerial::get();
    // LGSerial::put(c);

    // network.loop();
    // sleep(1000);
}
