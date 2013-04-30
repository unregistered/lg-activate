 /*
 *  controller_display.c
 *
 *
 *  Created by Student on 4/27/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */
#include <avr/io.h>
#include "adafruit335.c"
#include "controller_display.h"
#include "lgserial.h"
#include "lgui.h"

Controller::Controller() {
	manager = ScreenManager();
}

void Controller::setup()
{
	LGSerial::init();
	adafruit_initialize();

	manager.presentScreen(homeScreen);
}

void Controller::loop()
{
	manager.loop();
}