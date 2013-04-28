 /*
 *  controller_display.c
 *  
 *
 *  Created by Student on 4/27/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */
#include <avr/io.h>
//#include <avr/pgmspace.h> 
#include "adafruit335.c"
#include "controller_display.h"
#include "lgserial.h"
#include "adatouch.cpp"

Controller::Controller()
{
	
}

void Controller::setup()
{	
	LGSerial::init(); 
	LGSerial::put("Hello World!"); 
	DDRB |= LCD_Data_B; 
	DDRD |= 0xffff; 
	//PORTC |= LCD_Bits_C; 
	//DDRC |= LCD_Bits_C; 
	
	initialize(); 
	
	regout(0x50, 0);            // Set window to the full 240x320 screen
    regout(0x51, 239);
    regout(0x52, 0);
    regout(0x53, 319);
	
}

void Controller::loop()
{	
	homeMenu(); 
	while (1)
	{
	sleep(100); 
	DDRC = 0x0;  
	DDRC = 0x5; 
	TouchScreen ts = TouchScreen(10,10,10,10,10); 
	LGSerial::put("YM:"); 
		int y = ts.readTouchYM(); 
	LGSerial::print(y); 
	
	//DDRC |= 0xf; 
	DDRC = 0x0;
	DDRC = 0xa; 
	LGSerial::put("XM:"); 
		int x  = ts.readTouchXM(); 
	LGSerial::print(x); 
		x = ts.readTouchXM(); 
	sleep(100); 
		
		
		 if (x > 30 && x<100 && y>60 && y < 130)
		 {
		   devicesMenu(); 
			 homeMenu();
		 }
		 
		 else if (y> 140 && y<220 && x>30 && x<100)
		 {
		settingsMenu();
			 homeMenu();
		 }
		 else if (x>125 && x<215 &&y>60 && y<130)
		 {
			 statusMenu(); 
			homeMenu();
		 }
		 else if (x>125 && x<215 && y>140 && y<220) 
		 {
		   scheduleMenu();
			 homeMenu();
		 }	
		 
	}
}
