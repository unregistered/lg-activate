#include "controller_example.h"
#include <avr/io.h>
#include <avr/pgmspace.h>

// Variable initialization can be done here
Controller::Controller()
{
}

// This runs once during program startup
void Controller::setup()
{
	// Setup code here
    PORTD |= (1 << PD2);
    PORTD &= ~(1 << PD2);
}

// This runs continuously in a loop.
void Controller::loop()
{
    // Main code here
}
