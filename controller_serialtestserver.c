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
}

void Controller::loop()
{
    // LGSerial::put("Loop");
    network.loop();
    sleep(1000);
    LGSerial::put('a');
}
