#include "controller_serialtestserver.h"
#include "util.h"

Controller::Controller()
{
    network = LGNetwork();
}

void Controller::setup()
{
	LGSerial::init(); // (9830400/(16*9600))-1;

    network.set_mode(LGNETWORK_DISCOVER);

    // LGSerial::put('a');
}

void Controller::loop()
{
    network.loop();
    sleep(3000);
    // network.loop();
    // LGSerial::put("Loop");
    // char c = LGSerial::get();
    // LGSerial::put(c);

    // network.loop();
    // sleep(1000);
}
