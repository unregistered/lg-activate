#include "controller_serialtestserver.h"
#include "util.h"

Controller::Controller()
{
    network = LGNetwork();
}

void Controller::setup()
{
	LGSerial::init(); // (9830400/(16*9600))-1;

    network.setup_server();
}

void Controller::loop()
{
    // LGSerial::put("Loop");
}
