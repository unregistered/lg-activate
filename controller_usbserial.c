#include "controller_example.h"

// Variable initialization can be done here
Controller::Controller()
{
}

// This runs once during program startup
void Controller::setup()
{
	// Setup code here
    LGSerial::init(63);
    LGSerial::put("Hello World!");
}

// This runs continuously in a loop.
void Controller::loop()
{
    // Main code here
    char c = LGSerial::get();
    LGSerial::put("You sent: ");
    LGSerial::put(c);
    LGSerial::put("\r\n");
}