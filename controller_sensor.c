#include "controller_sensor.h"
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "lgserial.h"
#include "lgnetwork.h"
#include "lg_ssd.h"
#include "lgdb.h"
#define BUTTON_SYNC_TIME 1500

static LGNetwork network;
uint8_t system_mode;
uint8_t sensed_time;
bool button_press = 0;
uint8_t button_time = 0;

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
//		network.set_mode(LGNETWORK_DISCOVER);
		network.set_mode(LGNETWORK_OPERATE);

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
		// if(LGSerial::available()) {
		// 	bool match = network.scan_for_header("CMD");
		// }
		update_LED(system_mode);
		update_relay(system_mode);
		
		if (!bit_is_clear(PIND,5))	{//see motion
			PORTD |= 1 << PD6;//turn led on
		}
		if (bit_is_clear(PIND,5))	{//no motion
			PORTD &= ~(1 << PD6);//turn led off
		}
		
		//LOOK AT THIS CODE HERE AND MAKE SURE THAT IT WORKS AFTER WE FIX THE BUTTON!!!!!
		if (!(bit_is_clear(PINB,0))){
			_delay_ms(5000);
			if (!(bit_is_clear(PINB,0))){	
				spin_SSDs();
			}
		}
		
		
		
		
		/*
		if (button_press == 0 && bit_is_clear(PINB,0)){//no button pressed recently&switch button is pressed
			button_press = 1;
			button_time = millis();				
		}
		else{
			unsigned long current_time = millis();
			if(!bit_is_clear(PINB,0)){ // sync button pressed
				button_press = 0;
				if ((current_time - button_time) >= BUTTON_SYNC_TIME ) {
					sleep(250); //handles debouncing
					if(!bit_is_clear(PINB,0)){ // if button not pressed
						spin_SSDs();
						network.set_mode(LGNETWORK_DISCOVER);
					}
				}
			}
		}		*/
	}
}



//if motion is sensed
//	time = millis()

//wile millis()<time+1hour
//if millis()>time+1hour 
	//send "no motion" 