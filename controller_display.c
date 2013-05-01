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
#include "lgnetwork.h"
#include "lgui.c"
#include "lg_rtc.h"

Controller::Controller() {
	manager = ScreenManager();
	network = LGNetwork();
}

void Controller::setup()
{
	LGSerial::init();
	adafruit_initialize();
	ClockInit();

	// Initialize buttons
    DDRD &= ~(1 << DDD7); // Back
    DDRB &= ~(1 << DDB2); // Home

    scheduleScreen.device_idx = 0;
	manager.presentScreen(scheduleScreen);
}

void Controller::loop()
{
	manager.loop();
}
