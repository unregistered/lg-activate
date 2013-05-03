#include "controller_adapter.h"
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "lgserial.h"
#include "lgnetwork.h"
#include "lg_ssd.h"
#include "lgdb.h"
#define BUTTON_SYNC_TIME 1500
uint8_t button_press;
unsigned long button_time;

unsigned long last_command_sent;
bool last_motion_value;

bool shouldSpin;

ISR(TIMER0_OVF_vect)
{
	if(shouldSpin)
		spin_SSDs();
}

void display_short_address()
{
    update_ssd0(LGNetwork::myShortAddr % 10);
    update_ssd1(LGNetwork::myShortAddr / 10);
}

// Variable initialization can be done here
Controller::Controller()
{
	network = LGNetwork(); //populates addresses from eeprom if they exist
}

// This runs once during program startup
void Controller::setup()
{
	LGSerial::init();
	init_SSDs();

	cli();
	// Interrupt timer setup, for silky smooth animation during other events
	TCCR0B |= (1 << CS02) | (1 << CS00); // Clock divider (prescalar)
	TIMSK |= (1 << TOIE0); // Enable interrupt vector

	// Port setup
	DDRD &= ~(1 << DDD6); // motion sensor
	DDRB |= (1 << DDB0); // Sensor led
	sei();

	if ( LGNetwork::myShortAddr == 0xFF ){	//means address is not in memory
		shouldSpin = true;
		network.set_mode(LGNETWORK_DISCOVER);
	} else {
		shouldSpin = false;
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
		}
	}

	if(network.currentMode == LGNETWORK_DISCOVER_READY) {
		shouldSpin = false;
		display_short_address();

		network.set_mode(LGNETWORK_OPERATE);
	}

	if(network.currentMode == LGNETWORK_OPERATE) {
		if(bit_is_set(PIND, 6) && !last_motion_value) {
			// Transition from 0 to 1
			PORTB |= (1 << PB0);
			last_motion_value = true;

			LGSerial::put(LGNetwork::myShortAddr);
		} else if(bit_is_clear(PIND, 6) && last_motion_value) {
			// Transition from 1 to 0
			PORTB &= ~(1 << PB0);
			last_motion_value = false;
		}

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
					}
				}
			}
			else { // sync
				if ((current_time - button_time) > BUTTON_SYNC_TIME ) {
					shouldSpin = true;
					LGDB::write_address(0xff);

					network.force_disconnect(); // So we aren't interrupted later
					network.set_mode(LGNETWORK_DISCOVER);
				}
			}

		}

	}
}
