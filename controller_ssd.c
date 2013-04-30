#include "controller_ssd.h"
#include "lg_ssd.h"

// Variable initialization can be done here
Controller::Controller()
{
}

// This runs once during program startup
void Controller::setup()
{
	init_SSDs();

	//modify these to read from remote,
	//if data is not already contained in them
	int ssd0 = 0;
	int ssd1 = 0;
	int status = 1;	//refers to relay. 1 for on, 0 for off
	int read_ssd0 = 0;	//read ssd values from wireless
	int read_ssd1 = 0;
	int read_status = 1;
	update_ssd0(ssd0);
	update_ssd1(ssd1);

}//end setup

// This runs continuously in a loop.
void Controller::loop()
{


	//read from wireless, see if ssd or mode changes
	//if the ssd or mode is different from before,
	//update it

	//	_delay_ms(10000);

	//if button is pressed
	if ((bit_is_clear(PIND,5))){//PD5, the button pin, is pressed
		//check to see if still depressed for 5+ seconds:
		_delay_ms(5000);
		if ((bit_is_clear(PIND,5))){
			//button has been selected for 5+ seconds
			//send reset signal to base station
			update_LED(SYSTEM_SYNC);	//turn led blue

			update_relay(0);
			spin_SSDs();	//spin SSDs

			//after sending sync signal, read ssd values and status from base station
			//update ssds
			//update relay
			//update led to on or off
			update_LED(1);	//turn led green, for debgging, remove this
			update_relay(1);

		}
	}


/*
	//after checking button input, now we can check if receiving wireless
	if (0 == 1){	//read from wireless
		//read status
		//read ssd0
		//read ssd1
		if (read_status != status) {	//updating status of relay
			update_relay(read_status);
			update_LED(read_status);
			status = read_status;
		}
		if (read_ssd0 != ssd0) {
			update_ssd0(read_ssd0);
			ssd0 = read_ssd0;
		}
		if (read_ssd1 != ssd1) {
			update_ssd1(read_ssd1);
			ssd1 = read_ssd1;
		}
	}//end if read from wireless
*/

	_delay_ms(100);

	update_ssd0(9);
	update_ssd1(9);



}
