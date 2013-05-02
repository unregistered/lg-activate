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
	//SetHour(3); 
	//SetMinute(34); 
	//SetAmPm(0); 
	//SetYear(13); 
	//SetMonth(5); 
	//SetDay(2); 

	// Initialize buttons
    DDRD &= ~(1 << DDD7); // Back
    DDRB &= ~(1 << DDB2); // Home

    scheduleScreen.device_idx = 0;
    LGDB::write_device_table_entry(0, 0x00);
    for(uint8_t i=0; i < 7; i++)
	    LGDB::write_schedule_table_entry(0, i, 0x0000);

    LGDB::write_device_table_entry(1, 0x01);
    LGDB::write_device_table_entry(2, 0x01);

	manager.presentScreen(schedulePickDeviceScreen);
}

void Controller::loop()
{
	manager.loop();

	// if((millis() / 1000) % 30 == 0) {
	// 	if(network.currentMode == LGNETWORK_OPERATE)
	// 		network.loop();
	// }
}
