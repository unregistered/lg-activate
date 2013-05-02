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

unsigned long long last_looped = 0;

Controller::Controller() {
	manager = ScreenManager();
	network = LGNetwork();
}

void Controller::setup()
{
	LGSerial::init();
	adafruit_initialize();
	ClockInit();

	network.set_mode(LGNETWORK_OPERATE);

	// For calibrating time
	SetHour(4);
	SetMinute(14);
	SetAmPm(1);
	SetYear(13);
	SetMonth(5);
	SetDay(4);
	SetDate(2);

	// Initialize buttons
    DDRD &= ~(1 << DDD7); // Back
    DDRB &= ~(1 << DDB2); // Home

    scheduleScreen.device_idx = 0;
    LGDB::write_device_table_entry(0, 0x00);
    for(uint8_t i=0; i < 7; i++) {
	    LGDB::write_schedule_table_entry(0, i, 0x0043);
	    LGDB::write_sensor_table_entry(0, i, 2);
	}

    LGDB::write_device_table_entry(1, 0x00);
    for(uint8_t i=0; i < 7; i++) {
	    LGDB::write_schedule_table_entry(1, i, 0x4100);
	    LGDB::write_sensor_table_entry(1, i, 2);
	}

    LGDB::write_device_table_entry(2, 0x01);
    LGDB::write_device_table_entry(3, 0x01);

	manager.presentScreen(schedulePickDeviceScreen);
}

void Controller::loop()
{
	manager.loop();

	if(network.currentMode == LGNETWORK_OPERATE || LGSerial::available()) {
		if((millis() - last_looped) > 5000) {
			network.loop();
			last_looped = millis();
		}
	}
}
