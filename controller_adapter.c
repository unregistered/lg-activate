#include "controller_adapter.h"
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "lgserial.h"
#include "lgnetwork.h"
#include "lg_ssd.h"
#include "lgdb.h"
#define BUTTON_SYNC_TIME 1500
static LGNetwork network;
uint8_t button_press;
unsigned long button_time;

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
	update_LED(system_mode);
	update_relay(system_mode);

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
		network.loop();

		update_LED(system_mode);
		update_relay(system_mode);

		if (button_press == 0){
			if (bit_is_clear(PIND,5)){//switch button
				button_press = 1;
				button_time = millis();
			}

		}
		else{
			unsigned long current_time = millis();
			if(!bit_is_clear(PIND,5)){ // manual override
				button_press = 0;
				if ((current_time - button_time) < BUTTON_SYNC_TIME ) {
					sleep(250); //handles debouncing
					if(!bit_is_clear(PIND,5)){ // if button not pressed
						// on to off, off to on, auto to on
						system_mode = (system_mode == SYSTEM_ON) ? SYSTEM_OFF : SYSTEM_ON;
					}
				}
			}
			else { // sync
				if ((current_time - button_time) > BUTTON_SYNC_TIME ) {
					update_LED(SYSTEM_ON); // We want to default to "on" while we discover
					update_relay(SYSTEM_ON);
					network.set_mode(LGNETWORK_DISCOVER);
				}
			}

		}

	}
}
