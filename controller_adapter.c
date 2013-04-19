#include "controller_adapter.h"
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "lgserial.h"
#include "lgnetwork.h"
#include "lg_ssd.h"
#include "lgdb.h"
static LGNetwork network;
uint8_t mode;

// Variable initialization can be done here
Controller::Controller()
{
	network = LGNetwork();//populates addresses from eeprom if they exist
}

// This runs once during program startup
void Controller::setup()
{
	LGSerial::init();
	init_SSDs();
	
	if ( LGNetwork::myShortAddr == 0xFF ){	//means address is not in memory
		network.set_mode(LGNETWORK_DISCOVER);
		mode = 1;
	}
	else {
		mode = LGDB::read_mode();
	}
	update_LED(mode);
	update_relay(mode);	
}

// This runs continuously in a loop.
void Controller::loop()
{
	if (network.currentMode == LGNETWORK_DISCOVER) {
		spin_SSDs();
		network.loop();
	}
	
}
