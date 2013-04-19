#include "controller_example.h"
#include "util.h"
#include "lg_ssd.h"

// Variable initialization can be done here
Controller::Controller()
{
}

// This runs once during program startup
void Controller::setup()
{
	// Setup code here
    LGSerial::init();
    LGSerial::put("Hello World!");
    init_SSDs();
}

// This runs continuously in a loop.
void Controller::loop()
{
    // Main code here
    LGSerial::print(millis());

    sleep(100);
    // if(LGSerial::available()) {
    //     char c = LGSerial::get();
    //     LGSerial::put("You sent: ");
    //     LGSerial::put(c);
    //     LGSerial::put("\r\n");

    // } else {
    //     LGSerial::put("Nope\r\n");
    //     sleep(100);
    // }
}
