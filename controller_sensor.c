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

// Start at -3, since the header 'CMD' is 3 bytes. Once it reaches 0,
// then we can accept the packet data
int8_t packet_ptr = -3;
command_packet_t command_packet;

bool shouldSpin;

// During operation, receive commands in the ISR so we don't interrupt button pushing
void rx_intr_enable()
{
	cli();
	UCSRB |= (1 << RXCIE); // Interrupt enable
	sei();
}

void rx_intr_disable()
{
	cli();
	UCSRB &= ~(1 << RXCIE); // Interrupt enable
	sei();
}

ISR(USART_RX_vect)
{
    cli();

    char ch = UDR;
    if(ch == 'C' && packet_ptr == -3) {
    	packet_ptr++;
    } else if(ch == 'M' && packet_ptr == -2) {
    	packet_ptr++;
    } else if(ch == 'D' && packet_ptr == -1) {
    	packet_ptr++;
    } else if(packet_ptr >= 0) {
    	if(packet_ptr < sizeof(command_packet_t)) command_packet.bytes[packet_ptr++] = ch;
    } else {
    	packet_ptr = -3;
    }

    sei();
}

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

	// Interrupt timer setup, for silky smooth animation during other events
	cli();
	TCCR0B |= (1 << CS02) | (1 << CS00); // Clock divider (prescalar)
	TIMSK |= (1 << TOIE0); // Enable interrupt vector
	sei();

	if ( LGNetwork::myShortAddr == 0xFF ){	//means address is not in memory
		shouldSpin = true;
		network.set_mode(LGNETWORK_DISCOVER);
		system_mode = SYSTEM_SYNC;
	} else {
		shouldSpin = false;

		system_mode = LGDB::read_mode();
		if(system_mode == 0xF) { // Hasn't yet been set
			LGDB::write_mode(SYSTEM_ON);
		}

		display_short_address();
		network.set_mode(LGNETWORK_OPERATE);

		rx_intr_enable();
	}

	update_LED(system_mode);
	update_relay(system_mode);

}

// This runs continuously in a loop.
void Controller::loop()
{

	if (network.currentMode == LGNETWORK_DISCOVER) {
		update_LED(SYSTEM_SYNC);
		while(network.currentMode == LGNETWORK_DISCOVER) {
			network.loop(); // Will transition to DISCOVER_READY when done
		}
	}

	if(network.currentMode == LGNETWORK_DISCOVER_READY) {
		shouldSpin = false;
		display_short_address();

		system_mode = SYSTEM_ON;
		LGDB::write_mode(SYSTEM_ON);

		network.set_mode(LGNETWORK_OPERATE);

		rx_intr_enable();
	}

	if(network.currentMode == LGNETWORK_OPERATE) {
		if(packet_ptr == sizeof(command_packet_t)) {
			// We filled up the command pointer
			if(command_packet.packet.short_address == LGNetwork::myShortAddr) {
				system_mode = command_packet.packet.system_mode;
				LGDB::write_mode(system_mode);
			}

			packet_ptr = -3; // Open ourselves to more commands
		}

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
						LGDB::write_mode(system_mode);
					}
				}
			}
			else { // sync
				if ((current_time - button_time) > BUTTON_SYNC_TIME ) {
					shouldSpin = true;
					LGDB::write_address(0xff);
					update_LED(SYSTEM_SYNC); // We want to default to "on" while we discover
					update_relay(SYSTEM_ON);

					rx_intr_disable();

					network.force_disconnect(); // So we aren't interrupted later
					network.set_mode(LGNETWORK_DISCOVER);
				}
			}

		}

	}
}
