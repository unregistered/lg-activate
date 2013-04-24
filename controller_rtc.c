#include "controller_rtc.h"
#include "util.h"
#include "lg_rtc.h"


// Variable initialization can be done here
Controller::Controller()
{
}

// This runs once during program startup
void Controller::setup()
{
	// Setup code here
    LGSerial::init();
    LGSerial::put("Hello World!");
	
	DDRC |= 1 << DDC2;//trigger
	DDRC |= 1 << DDC3;//pc3 pin 26 for output
	DDRC |= 1 << DDC4;
	DDRC |= 1 << DDC5;
	DDRD |= 1 << DDD0;
	
	PORTD &= ~(1 << PD0);
	PORTC &= ~(1 << PC2);
	PORTC &= ~(1 << PC3);
	_delay_ms(500);
	
	ClockInit();

	
	SetSecond(1);
	SetMinute(33);
	SetHour(8);
	SetAmPm(1);///0 for AM, 1 for PM
	SetDay(3);
	SetDate(23);
	SetMonth(4);
	SetYear(13);
	
	char Time[12];	//hh:mm:ss AM/PM
	
	while(0)
	{
		//Get the Current Time as a String
		if(!GetTimeString(Time))//modifies time
		{
			/*
			 If return value is false then some error has occured
			 
			 Check
			 ->DS1307 Installed Properly
			 ->DIP Switch 1,2 are in on position
			 */
			while(1);//halt
		}
		_delay_ms(500);
	}
	
	uint8_t second = GetSecond();
	uint8_t minute = GetMinute();
	uint8_t hour = GetHour();
	uint8_t AmPm = GetAmPm();
	uint8_t day = GetDay();
	uint8_t date = GetDate();
	uint8_t month = GetMonth();
	uint8_t year = GetYear();
	
	
	LGSerial::print(second);
	LGSerial::print(minute);
	LGSerial::print(hour);
	LGSerial::print(AmPm);
	LGSerial::print(day);
	LGSerial::print(date);
	LGSerial::print(month);
	LGSerial::print(year);
	
	_delay_ms(20000);
	second = GetSecond();
	minute = GetMinute();
	LGSerial::print(second);
	LGSerial::print(minute);
	
	_delay_ms(20000);
	second = GetSecond();
	minute = GetMinute();
	LGSerial::print(second);
	LGSerial::print(minute);
	
	
	
	//uint8_t minute = GetMinute();
//uint8_t minute = 0xAA;
//	bool min[8];
/*	
	min[0] = minute & 0x01;
	min[1] = minute & 0x02;
	min[2] = minute & 0x04;
	min[3] = minute & 0x08;
	min[4] = minute & 0x10;
	min[5] = minute & 0x20;
	min[6] = minute & 0x40;
	min[7] = minute & 0x80;
*/	
 
	/*
	 min[0] = 0;
	 min[1] = 1;
	 min[2] = 0;
	 min[3] = 1;
	 min[4] = 0;
	 min[5] = 1;
	 min[6] = 0;
	 min[7] = 1;
	 */

	/*
		
	int i = 0;
	
	while(1){	
		//make pc2 trigger
		PORTC |= 1 << PC2;
		PORTC &= ~(1 << PC2);	
		
		PORTC |= 1 << PC3;
		PORTC &= ~(1 << PC3);
		PORTC |= 1 << PC3;
		PORTC &= ~(1 << PC3);
		PORTC |= 1 << PC3;
		PORTC &= ~(1 << PC3);
		
		for (i=0;i<8;i++){
			if (min[i] == 1){	//if that bit of min is 1
				PORTD |= 1 << PD0;
				PORTD &= ~(1 << PD0);
				PORTD |= 1 << PD0;
				PORTD &= ~(1 << PD0);
				
				PORTC &= ~(1 << PC3);
				PORTC |= 1 << PC3;
				PORTC &= ~(1 << PC3);
				
				
			}
			else {//if (min[i] == 0){	//if that bit of min is 0
				PORTD |= 1 << PD0;
				PORTD &= ~(1 << PD0);
				
				PORTC |= 1 << PC3;
				PORTC &= ~(1 << PC3);
				PORTC |= 1 << PC3;
				
			}
		}
		
	}
	*/
}

// This runs continuously in a loop.
void Controller::loop()
{
    // Main code here

}
