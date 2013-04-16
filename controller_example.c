#include "controller_example.h"
#include <avr/io.h>

// Variable initialization can be done here
Controller::Controller()
{
}

// This runs once during program startup
void Controller::setup()
{
	// Setup code here
    PORTC &= PORTC;
}

// This runs continuously in a loop.
void Controller::loop()
{
    // Main code here
}
