/*
 *  adatouch.h
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

#ifndef _ADAFRUIT_TOUCHSCREEN_H_
#define _ADAFRUIT_TOUCHSCREEN_H_

#include <stdint.h>

class Point 
{
public:
	Point(void);
	Point(int16_t x, int16_t y, int16_t z);
	
	bool operator==(Point);
	bool operator!=(Point);
	
	uint16_t x, y, z;
};

class TouchScreen {
public:
	TouchScreen(uint8_t xp, uint8_t yp, uint8_t xm, uint8_t ym);
	TouchScreen(uint8_t xp, uint8_t yp, uint8_t xm, uint8_t ym, uint16_t rx);
	
	bool isTouching(void);
	uint16_t pressure(void);
	int readTouchYM(); //pc3
	int readTouchXM(); //pc2
	int readTouchXP(); //pc0
	int readTouchYP(); //pc1
	//void getPoint();
	int16_t pressureThreshhold;
	
private:
	uint8_t _yp, _ym, _xm, _xp;
	uint16_t _rxplate;
};

#endif
