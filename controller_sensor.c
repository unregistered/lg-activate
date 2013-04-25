#include "controller_sensor.h"
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "lgserial.h"
#include "lgnetwork.h"
#include "lg_ssd.h"
#include "lgdb.h"

static LGNetwork network;
uint8_t system_mode;

void display_short_address()
{
    update_ssd0(LGNetwork::myShortAddr % 10);
    update_ssd1(LGNetwork::myShortAddr / 10);
}

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
	} else {
		system_mode = LGDB::read_mode();
		display_short_address();
		network.set_mode(LGNETWORK_OPERATE);
	}
}

// This runs continuously in a loop.
void Controller::loop()
{
	if (network.currentMode == LGNETWORK_DISCOVER) {
		while(network.currentMode == LGNETWORK_DISCOVER) {
			network.loop(); // Will transition to DISCOVER_READY when done
			spin_SSDs();
		}
	}

	if(network.currentMode == LGNETWORK_DISCOVER_READY) {
		display_short_address();
		network.set_mode(LGNETWORK_OPERATE);
	}

	if(network.currentMode == LGNETWORK_OPERATE) {


	}
}
