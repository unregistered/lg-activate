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

	DDRB |= 1 << DDB0;
	DDRB |= 1 << DDB1;
	DDRB |= 1 << DDB2;
	DDRD |= 1 << DDD0;
	DDRD |= 1 << DDD1;
	DDRD |= 1 << DDD2;
	DDRD |= 1 << DDD3;
	DDRD |= 1 << DDD4;
	DDRD &= ~(1 << DDD5);		//set PORTD bit 5 for input(?)

	
	//set io pins to 0 or 1
	PORTB &= ~(1 << PB0); // Set LEDR to 0 
  PORTB &= ~(1 << PB1); //LEDG to 0
  PORTB &= ~(1 << PB2); //LEDB to 0

	PORTD &= ~(1 << PD0);  
	PORTD &= ~(1 << PD1);  
	PORTD |= 1 << PD2;  //PD2 clears shift registers, want set to 1
	PORTD &= ~(1 << PD3);  
	PORTD &= ~(1 << PD4); 	
	
	//modify these to read from remote,
	  //if data is not already contained in them
//  mode = 0;
 int ssd0 = 0;
 int ssd1 = 0;
 int new_ssd0;
 int new_ssd1;  
 	update_ssd0(ssd0);
	update_ssd1(ssd1);
  int mode = 0;//off

  update_LED(mode);

  
}//end setup

// This runs continuously in a loop.
void Controller::loop()
{
    // Main code here
    //read from wireless, see if ssd or mode changes
    //if the ssd or mode is different from before, 
      //update it

  ssd0 = 0;
  ssd1 = 1;
   
  update_ssd0(ssd0);
  update_ssd1(ssd1); 
  sleep(1000);
   
  ssd0 = 1;
  ssd1 = 2;
   
  update_ssd0(ssd0);
  update_ssd1(ssd1);   
  sleep(1000);
  
  ssd0 = 2;
  ssd1 = 3;
   
  update_ssd0(ssd0);
  update_ssd1(ssd1); 
  sleep(1000);
   
  ssd0 = 3;
  ssd1 = 4;
   
  update_ssd0(ssd0);
  update_ssd1(ssd1);   
  sleep(1000);
  
  update_ssd0(0);
  update_ssd1(1);
    sleep(1000);
  update_ssd0(1);
  update_ssd1(2);
    sleep(1000);
  update_ssd0(2);
  update_ssd1(3);
    sleep(1000);
  update_ssd0(3);
  update_ssd1(4);
    sleep(1000);
  update_ssd0(4);
  update_ssd1(5);
    sleep(1000);
  update_ssd0(5);
  update_ssd1(6);
    sleep(1000);
  update_ssd0(6);
  update_ssd1(7);
    sleep(1000);
  update_ssd0(7);
  update_ssd1(8);
    sleep(1000);
  update_ssd0(8);
  update_ssd1(9);
    sleep(1000);
  update_ssd0(9);
  update_ssd1(0);
    sleep(1000);
  
  update_ssd0(8);
  update_ssd1(8);
    sleep(1000);
  
       

  mode = 1;
  update_LED(mode);
	sleep(1000);
	
	mode = 2;
  update_LED(mode);
	sleep(1000);
	
	mode = 0;
  update_LED(mode);
  sleep(1000);
    
}