#include "controller_adapter.h"
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "lgserial.h"
#include "lgnetwork.h"
#include "lg_ssd.h"
static LGNetwork network;

// Variable initialization can be done here
Controller::Controller()
{
	network = LGNetwork();//populates addresses from eeprom if they exist
}

// This runs once during program startup
void Controller::setup()
{
	LGSerial::init();
	return;
	LGSerial::init();
	init_SSDs();
	
	if ( LGNetwork::myShortAddr == 0xFF ){	//means address is not in memory
		network.set_mode(LGNETWORK_DISCOVER);
	}
}

// This runs continuously in a loop.
void Controller::loop()
{
	//spin_SSDs();
	LGSerial::print(millis());
}
