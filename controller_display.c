/*
 *  controller_display.c
 *  
 *
 *  Created by Student on 4/27/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */
#include <avr/io.h>
#include <avr/pgmspace.h> 
#include "adafruit335.c"
#include "controller_display.h"

Controller::Controller()
{
  
}

void Controller::setup()
{
	DDRB |= LCD_Data_B; 
	DDRD |= 0xffff; 
	PORTC |= LCD_Bits_C; 
	DDRC |= LCD_Bits_C; 
	
	initialize(); 
	
	regout(0x50, 0);            // Set window to the full 240x320 screen
    regout(0x51, 239);
    regout(0x52, 0);
    regout(0x53, 319);
	
}

void Controller::loop()
{
	homeMenu(); 
}
