#include "controller_serialtestclient.h"
#include "lg_ssd.h"
#include "util.h"

// Variable initialization can be done here
Controller::Controller()
{
    network = LGNetwork();
}

// This runs once during program startup
void Controller::setup()
{
	// Setup code here
    init_SSDs();
    LGSerial::init();

    network.set_mode(LGNETWORK_DISCOVER);
}

// This runs continuously in a loop.
void Controller::loop()
{
    while(network.currentMode == LGNETWORK_DISCOVER) {
        network.loop();
    }

    // LGSerial::print_hex(LGNetwork::baseUUID);
    LGSerial::print(StackCount());

    while(true);
    // while(true) ;
    // char c = LGSerial::get();
    // LGSerial::put(c);

    // Main code here
    // char buf[5];
    // byte_to_asciis(buf, LGNetwork::myShortAddr);
    // buf[4] = 0;
    // buf[2] = '\r';
    // buf[3] = '\n';
    // LGSerial::print(buf);

    // if(LGNetwork::myShortAddr == 0xff) {
    //     // We still done't have an address
    //     LGSerial::print("No luck");
    //     network.loop();
    //     sleep(100);
    //     update_ssd0(0);
    //     update_ssd1(0);
    // } else {
    //     // We were just assigned an address
    //     // network.set_mode(LGNETWORK_OPERATE);
    //     // Display SSDs

    //     if(LGSerial::available()) {
    //         // We should be given a mode
    //         // uint8_t mode = LGSerial::get();
    //         // Do things with mode
    //         sleep(100);
    //         update_ssd0(9);
    //         update_ssd1(8);
    //     }
    // }

    // LGSerial::put('a');
    // sleep(1000);
}
