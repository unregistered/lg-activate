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
    // LGDB::write_device_table_entry(0, 0x00);
    // LGDB::write_schedule_table_entry(0, 0, 0x5C03);
    // LGSerial::print(LGDB::read_schedule_table_entry(0, 0));
    // LGSerial::print(LGDB::read_hour(0, 0, true));
    // LGSerial::print(LGDB::read_minute(0, 0, true));
    // for(int i=0; i < 8; i++) {
	   //  LGDB::incr_time(0, 0, true);
	   //  LGSerial::print(LGDB::read_schedule_table_entry(0, 0));
	   //  LGSerial::print(LGDB::read_hour(0, 0, true));
	   //  LGSerial::print(LGDB::read_minute(0, 0, true));
    // 	sleep(100);
    // }

	manager.presentScreen(scheduleScreen);
}

void Controller::loop()
{
	// LGSerial::put("Raw:");
	// LGSerial::print(LGDB::read_schedule_table_entry(0, 0));
	// LGSerial::put("Hr:");
 //    LGSerial::print(LGDB::read_hour(0, 0, true));
 //    LGSerial::put("Mn:");
 //    LGSerial::print(LGDB::read_minute(0, 0, true));

 //    LGDB::decr_time(0, 0, true);

	// sleep(500);
	manager.loop();
}
