#include "controller_ssd.h"
#include "lg_ssd.h"

// Variable initialization can be done here
Controller::Controller()
{
}

// This runs once during program startup
void Controller::setup()
{
	// Setup code here

    /* insert your hardware initialization here */
	DDRB |= 1 << DDB0;
	DDRB |= 1 << DDB1;
	DDRB |= 1 << DDB2;
	DDRD |= 1 << DDD0;
	DDRD |= 1 << DDD1;
	DDRD |= 1 << DDD2;
	DDRD |= 1 << DDD3;
	DDRD |= 1 << DDD4;
	DDRD &= ~(1 << DDD5);		//set PORTD bit 5 for input(?)
	DDRD |= 1 << DDD6;//for testing reading the  button
	
	//set io pins to 0 or 1
	PORTB &= ~(1 << PB0); // Set LEDR to 0
	PORTB &= ~(1 << PB1); //LEDG to 0
	PORTB &= ~(1 << PB2); //LEDB to 0
	
	PORTD &= ~(1 << PD0);
	PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD2;  //PD2 clears shift registers, want set to 1
	PORTD &= ~(1 << PD3);
	PORTD &= ~(1 << PD4);
	PORTD &= ~(1 << PD6);
	
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
	
	
	update_LED(status);
	

  
}//end setup

// This runs continuously in a loop.
void Controller::loop()
{
    /* insert your main loop code here */
	// Main code here
	//read from wireless, see if ssd or mode changes
	//if the ssd or mode is different from before,
	//update it
	
	_delay_ms(10000);
	
	//if button is pressed
	if (bit_is_clear(PIND,5)){//PD5, the button pin, is pressed
		//check to see if still depressed for 5+ seconds:
		_delay_ms(7000);
		if (bit_is_clear(PIND,5)){
			//button has been selected for 5+ seconds
			spin_SSDs();	//spin SSDs
			//send reset signal to base station
			PORTD &= ~(1 << PD6);	//turns LED (PD6) on
		}
		else {
			PORTD |= 1 << PD6;	//turns LED off
		}
	}
	else{
		PORTD |= 1 << PD6;	//turns LED off
	}
	
	
	
	//after checking button input, now we can check if receiving wireless
	////read from wireless
	//read status (on/off) 
	//read ssd0
	//read ssd1
	
	
	
	if (read_status != status) {	//updating status of relay
		update_relay(read_status);
		status = read_status;
	}
	if (read_ssd0 != ssd0) {
		update_ssd0(read_ssd0);
		ssd0 = read_ssd0;
	}
	if (read_ssd1 != ssd1){
		update_ssd1(read_ssd1);
		ssd1 = read_ssd1;
	}
	
	
	
	
	
	/*
	 ssd0 = 1;
	 ssd1 = 1;
	 
	 update_ssd0(ssd0);
	 update_ssd1(ssd1);
	 _delay_ms(1000);
	 
	 update_ssd0(0);
	 update_ssd1(0);
	 _delay_ms(1000);
	 
	 update_ssd0(1);
	 update_ssd1(1);
	 _delay_ms(1000);
	 update_ssd0(2);
	 update_ssd1(2);
	 
	 _delay_ms(1000);
	 update_ssd0(3);
	 update_ssd1(3);
	 
	 _delay_ms(1000);
	 update_ssd0(4);
	 update_ssd1(4);
	 
	 
	 _delay_ms(1000);
	 update_ssd0(5);
	 update_ssd1(5);
	 
	 _delay_ms(1000);
	 update_ssd0(6);
	 update_ssd1(6);
	 
	 _delay_ms(1000);
	 update_ssd0(7);
	 update_ssd1(7);
	 
	 _delay_ms(1000);
	 update_ssd0(8);
	 update_ssd1(8);
	 
	 _delay_ms(1000);
	 update_ssd0(9);
	 update_ssd1(9);
	 
	 _delay_ms(1000);
	 
	 //	update_ssd0(8);
	 //	update_ssd1(8);
	 
	 
	 mode = 1;
	 update_LED(mode);
	 //sleep(1000);
	 _delay_ms(1000);
	 
	 
	 mode = 2;
	 update_LED(mode);
	 //sleep(1000);
	 _delay_ms(1000);
	 
	 
	 mode = 0;
	 update_LED(mode);
	 //sleep(1000);
	 _delay_ms(1000);
	 */
	
	}
    
}