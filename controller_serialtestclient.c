#include "controller_serialtestclient.h"

// Variable initialization can be done here
Controller::Controller()
{
    network = LGNetwork();
}

// This runs once during program startup
void Controller::setup()
{
	// Setup code here
    LGSerial::init();

    network.set_mode(LGNETWORK_DISCOVER);

    // network.loop();
}

// This runs continuously in a loop.
void Controller::loop()
{
    // Main code here
    sleep(1000);
    network.loop();

    if(LGNetwork::myShortAddr == -1) {
        // We still done't have an address
    } else {
        // We were just assigned an address
        network.set_mode(LGNETWORK_OPERATE);

    }

    // LGSerial::put('a');
    // sleep(1000);
}
