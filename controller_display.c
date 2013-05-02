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
#include "lgdb.h"

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
    LGDB::write_device_table_entry(0, 0x00);
    // LGDB::write_schedule_table_entry(0, 0, 0x1313);
	manager.presentScreen(scheduleScreen);
}

void Controller::loop()
{
	manager.loop();
	LGSerial::print(StackCount());
}
