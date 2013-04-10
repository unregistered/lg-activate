#include "controller_serialtestclient.h"

// Variable initialization can be done here
Controller::Controller()
{
}

// This runs once during program startup
void Controller::setup()
{
	// Setup code here
    LGSerial::init();
}

// This runs continuously in a loop.
void Controller::loop()
{
    // Main code here
    LGSerial::put('u');
    sleep(100);
}
