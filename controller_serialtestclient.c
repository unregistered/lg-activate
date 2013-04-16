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

    if(LGNetwork::myShortAddr == -1) {
        // We still done't have an address
        network.loop();
    } else {
        // We were just assigned an address
        // network.set_mode(LGNETWORK_OPERATE);
        // Display SSDs

        if(LGSerial::available()) {
            // We should be given a mode
            uint8_t mode = LGSerial::get();
            // Do things with mode

        }
    }

    // LGSerial::put('a');
    // sleep(1000);
}
