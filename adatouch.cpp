/*
 *  adatouch.c
 *  
 *
 *  Created by Student on 4/18/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

// Touch screen library with X Y and Z (pressure) readings as well
// as oversampling to avoid 'bouncing'
// (c) ladyada / adafruit
// Code under MIT License

//#include "pins_arduino.h"
//#include "wiring_private.h"
#include <avr/pgmspace.h>
//#include "TouchScreen.h"

#define XM (1 << PC0); 
#define XP (1 << PC1); 
#define YM (1 << PC2); 
#define YP (1 << PC3); 

#define 


// #include http://code.google.com/p/arduino/source/browse/trunk/hardware/arduino/cores/arduino/?r=972 // 
// increase or decrease the touchscreen oversampling. This is a little different than you make think:
// 1 is no oversampling, whatever data we get is immediately returned
// 2 is double-sampling and we only return valid data if both points are the same
// 3+ uses insert sort to get the median value.
// We found 2 is precise yet not too slow so we suggest sticking with it!

#define NUMSAMPLES 2

Point::Point(void) {
  x = y = 0;
}

Point::Point(int16_t x0, int16_t y0, int16_t z0) {
	x = x0;
	y = y0;
	z = z0;
}

bool Point::operator==(Point p1) {
	return  ((p1.x == x) && (p1.y == y) && (p1.z == z));
}

bool Point::operator!=(Point p1) {
	return  ((p1.x != x) || (p1.y != y) || (p1.z != z));
}

#if (NUMSAMPLES > 2)
static void insert_sort(int array[], uint8_t size) 
{
	uint8_t j;
	int save;
	
	for (int i = 1; i < size; i++) {
		save = array[i];
		for (j = i; j >= 1 && save < array[j - 1]; j--)
			array[j] = array[j - 1];
		array[j] = save; 
	}
}
#endif

Point TouchScreen::getPoint(void) {
	int x, y, z;
	int samples[NUMSAMPLES]; // 2 
	uint8_t i, valid;
	
	/*
	uint8_t xp_port = digitalPinToPort(_xp); // defined pins_arduino.h // // return port of specified pin
	uint8_t yp_port = digitalPinToPort(_yp); // defined pins_arduino.h // // return port of specified pin
	uint8_t xm_port = digitalPinToPort(_xm); // defined pins_arduino.h // 
	uint8_t ym_port = digitalPinToPort(_ym); // defined pins_arduino.h // 	
	*/ 
	/*
	uint8_t xp_port = pgm_read_byte(); 
	uint8_t yp_port = pgm_read_byte(); 
	uint8_t xm_port = pgm_read_byte();
	uint8_t ym_port = pgm_read_byte();
	*/
	/*
	uint8_t xp_pin = digitalPinToBitMask(_xp);  // defined pins_arduino.h // 
	uint8_t yp_pin = digitalPinToBitMask(_yp);	// return bitmask of specified pin // 
	uint8_t xm_pin = digitalPinToBitMask(_xm);	// return bitmask of specified pin // 
	uint8_t ym_pin = digitalPinToBitMask(_ym);
	*/
	/*
	uint8_t xp_pin = pgm_read_byte(); 
	uint8_t yp_pin = pgm_read_byte();
	uint8_t xm_pin = pgm_read_byte();
	uint8_t ym_pin = pgm_read_byte();
	*/
	// def digitalPintoPort // 
	
	//const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
    //    PD, /* 0 */
     //   PD,
    //    PD,
   //     PD,
   //     PD,
    //    PD,
     //   PD,
     //   PD,
    //    PB, /* 8 */
    //    PB,
     //   PB,
    //    PB,
     //   PB,
    //    PB,
    //    PC, /* 14 */
    //    PC,
    //    PC,
    //    PC,
    //    PC,
    //    PC,
//	};
	 
	// def digitalpintobitmask // 
	/*const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
        _BV(0), //Port D
        _BV(1),
        _BV(2),
        _BV(3),
        _BV(4),
        _BV(5),
        _BV(6),
        _BV(7),
        _BV(0), // Port B
        _BV(1),
        _BV(2),
        _BV(3),
        _BV(4),
        _BV(5),
        _BV(0), // Port C 
        _BV(1),
        _BV(2),
        _BV(3),
        _BV(4),
        _BV(5),
	};*/
	
	
	valid = 1;
	
	// Configure y+/y- to inputs ?
	
	//pinMode(_yp, INPUT); // wiring digital . c // 
	
	//pinMode(_ym, INPUT); // wiring digital . c // 
	DDRC |= 0x0; 
	
	//*portOutputRegister(yp_port) &= ~yp_pin; // pins_arduino.h //  // return output port register of ys// 
	//*portOutputRegister(ym_port) &= ~ym_pin; // pins_arduion.h // 
	PORTC &= ~YP; 
	PORTC &= ~YM; 
	
	
	//digitalWrite(_yp, LOW);
	//digitalWrite(_ym, LOW);
	
	// configure x+/x- as outputs ? 
	
	//pinMode(_xp, OUTPUT); // wiring digital . c 
	//pinMode(_xm, OUTPUT); // wiring digital . c 
	DDRC |= 0x0; 
	
	
	//digitalWrite(_xp, HIGH);
	//digitalWrite(_xm, LOW);
	
	//*portOutputRegister(xp_port) |= xp_pin; // pins_arduino.h // // return output port registers for xs //  
	//*portOutputRegister(xm_port) &= ~xm_pin;
	PORTC |= XP; 
	PORTC &= ~XM; 
	
	for (i=0; i<NUMSAMPLES; i++) {
		// read value of analog pin using A-D Converter // 
		ADMUX |= (1<<REFS0); 
		ADMUX &= ~(1<<REFS1); 
		
		ADMUX |= (1<<ADLAR); 
		
		ADMUX |= (3 << MUX0); 
		ADMUX &= (0xf0 | (3<<MUX0)); 
		
		ADCSRA |= (7<<ADPS0); 
		ADCSRA |= (1<<ADEN); 
		
		ADCSRA |= (1<<ADSC); 
		
		while (ADSCRA& (1<<ADSC)); 
		
		samples[i] = ADCH; 
	}
	
	
	
/*	
#if NUMSAMPLES > 2
	insert_sort(samples, NUMSAMPLES);
#endif*/
#if NUMSAMPLES == 2
	if (samples[0] != samples[1]) { valid = 0; }
#endif
	x = (1023-samples[NUMSAMPLES/2]);
	
	// Configure x+/x0 as inputs ? 
	
	//pinMode(_xp, INPUT); // wiring digital . c // 
	//pinMode(_xm, INPUT);
	DDRC |= 0; 
	// returen output port registers of x+ and y+ // 
	//*portOutputRegister(xp_port) &= ~xp_pin;
	PORTC &= ~XP; 
	
	//digitalWrite(_xp, LOW);
	
	// configure y+/y- as outputs // 
	
	//pinMode(_yp, OUTPUT); // wiring digital .c // 
	DDRC |= 0x8; 
	//*portOutputRegister(yp_port) |= yp_pin;
	PORTC |= YP; 
	//digitalWrite(_yp, HIGH);
	//pinMode(_ym, OUTPUT);
	DDRC |= 0x4; 
	for (i=0; i<NUMSAMPLES; i++) {
		// read value of analog pin using A-D converter // 
		//samples[i] = analogRead(_xm); // wiring analog .c // 
		ADMUX |= (1<<REFS0); 
		ADMUX &= ~(1<<REFS1); 
		
		ADMUX |= (1<<ADLAR); 
		
		ADMUX |= (0 << MUX0); 
		ADMUX &= (0xf0 | (0<<MUX0)); 
		
		ADCSRA |= (7<<ADPS0); 
		ADCSRA |= (1<<ADEN); 
		
		ADCSRA |= (1<<ADSC); 
		
		while (ADSCRA& (1<<ADSC)); 
		
		samples[i] = ADCH;
	}
/*
#if NUMSAMPLES > 2
	insert_sort(samples, NUMSAMPLES);
#endif*/
#if NUMSAMPLES == 2
	if (samples[0] != samples[1]) { valid = 0; }
#endif
	
	y = (1023-samples[NUMSAMPLES/2]);
	
	// Set X+ to ground
	pinMode(_xp, OUTPUT);
	*portOutputRegister(xp_port) &= ~xp_pin;
	//digitalWrite(_xp, LOW);
	
	// Set Y- to VCC
	*portOutputRegister(ym_port) |= ym_pin;
	//digitalWrite(_ym, HIGH); 
	
	// Hi-Z X- and Y+
	*portOutputRegister(yp_port) &= ~yp_pin;
	//digitalWrite(_yp, LOW);
	pinMode(_yp, INPUT);
	
	int z1 = analogRead(_xm); 
	int z2 = analogRead(_yp);
	
	if (_rxplate != 0) {
		// now read the x 
		float rtouch;
		rtouch = z2;
		rtouch /= z1;
		rtouch -= 1;
		rtouch *= x;
		rtouch *= _rxplate;
		rtouch /= 1024;
		
		z = rtouch;
	} else {
		z = (1023-(z2-z1));
	}
	
	if (! valid) {
		z = 0;
	}
	
	return Point(x, y, z);
}

TouchScreen::TouchScreen(uint8_t xp, uint8_t yp, uint8_t xm, uint8_t ym) {
	DDRC = 0x00;  
	_yp = yp;
	_xm = xm;
	_ym = ym;
	_xp = xp;
	_rxplate = 0;
	pressureThreshhold = 10;
}


TouchScreen::TouchScreen(uint8_t xp, uint8_t yp, uint8_t xm, uint8_t ym,
						 uint16_t rxplate) {
	DDRC = 0x00; 
	_yp = yp;
	_xm = xm;
	_ym = ym;
	_xp = xp;
	_rxplate = rxplate;
	
	pressureThreshhold = 10;
}

int TouchScreen::readTouchX(void) {
	pinMode(_yp, INPUT);
	pinMode(_ym, INPUT);
	digitalWrite(_yp, LOW); // wiring digital.c 
	digitalWrite(_ym, LOW);
	
	pinMode(_xp, OUTPUT);
	digitalWrite(_xp, HIGH);
	pinMode(_xm, OUTPUT);
	digitalWrite(_xm, LOW);
	
	return (1023-analogRead(_yp));
}


int TouchScreen::readTouchY(void) {
	pinMode(_xp, INPUT);
	pinMode(_xm, INPUT);
	digitalWrite(_xp, LOW);
	digitalWrite(_xm, LOW);
	
	pinMode(_yp, OUTPUT);
	digitalWrite(_yp, HIGH);
	pinMode(_ym, OUTPUT);
	digitalWrite(_ym, LOW);
	
	return (1023-analogRead(_xm));
}


uint16_t TouchScreen::pressure(void) {
	// Set X+ to ground
	pinMode(_xp, OUTPUT);
	digitalWrite(_xp, LOW);
	
	// Set Y- to VCC
	pinMode(_ym, OUTPUT);
	digitalWrite(_ym, HIGH); 
	
	// Hi-Z X- and Y+
	digitalWrite(_xm, LOW);
	pinMode(_xm, INPUT);
	digitalWrite(_yp, LOW);
	pinMode(_yp, INPUT);
	
	int z1 = analogRead(_xm); 
	int z2 = analogRead(_yp);
	
	if (_rxplate != 0) {
		// now read the x 
		float rtouch;
		rtouch = z2;
		rtouch /= z1;
		rtouch -= 1;
		rtouch *= readTouchX();
		rtouch *= _rxplate;
		rtouch /= 1024;
		
		return rtouch;
	} else {
		return (1023-(z2-z1));
	}
}
