#include "lgui.h"
#include "lg_rtc.h"
#include "lgdb.h"

static void twodigit(char *str, uint8_t num, bool shouldNullTerminate = true)
{
    str[0] = (num / 10) + 48;
    str[1] = (num % 10) + 48;
    if(shouldNullTerminate)
        str[2] = '\0';
}

ScreenManager manager;

HomeScreen homeScreen; // Links to Settings, Status, Devices, Schedule
StatusScreen statusScreen;
DeviceScreen deviceScreen;
DeviceAddScreen deviceAddScreen;
DeviceRemoveScreen deviceRemoveScreen;
SettingsScreen settingsScreen; // Links to Set Time, Set Mode
SettingsSetTimeScreen settingsSetTimeScreen;
SettingsSetModeScreen settingsSetModeScreen;
SchedulePickDeviceScreen schedulePickDeviceScreen; // Links to schedule
ScheduleScreen scheduleScreen;

ScreenManager::ScreenManager() : currentScreen(NULL), history_idx(0)
{
    homeScreen = HomeScreen();
    statusScreen = StatusScreen();
    deviceScreen = DeviceScreen();
    deviceAddScreen = DeviceAddScreen();
    deviceRemoveScreen = DeviceRemoveScreen();
    settingsScreen = SettingsScreen();
    settingsSetModeScreen = SettingsSetModeScreen();
    settingsSetTimeScreen = SettingsSetTimeScreen();
    schedulePickDeviceScreen = SchedulePickDeviceScreen();
    scheduleScreen = ScheduleScreen();
}

void ScreenManager::presentScreen(LGUIScreen &s)
{
    if(currentScreen) {
        currentScreen->beforeExit();
    }

    if(&s == &homeScreen) {
        // We should clear the history stack
        history_idx = 0;
    }

    history[history_idx++] = &s;

    s.beforeRender(); // hooks
    s.render();
    s.afterRender(); // hooks

    currentScreen = &s;
}

void ScreenManager::loop()
{
    // Do things common to all screens
    bool backButton = bit_is_set(PIND, 7);
    bool homeButton = bit_is_set(PINB, 2);

    if(backButton) {
        if(history_idx > 1) {
            LGUIScreen *screen = history[history_idx-2]; // -1 is ourself
            history_idx-=2; // Pop from the stack
            presentScreen(*screen);
        }
    } else if( homeButton && (currentScreen != &homeScreen) ) {
        presentScreen(homeScreen);
    } else {
        if(currentScreen)
            currentScreen->loop();
    }
}

HomeScreen::HomeScreen(){}
void HomeScreen::renderTime()
{
    uint8_t hour = GetHour();
    uint8_t minute = GetMinute();
    displayedTime = 4*hour + minute;

    char tim[] = "00:00 PM";
    twodigit(tim, hour, false);
    twodigit(tim+3, minute, false);
    if (GetAmPm() == 0)
    {
        tim[6] = 'A';
    }

    drawString(225, 220, tim, WHITE, BLACK, 2);
}
void HomeScreen::render()
{
    uint16_t schcolor = color565(142,35,35); //red
    uint16_t setcolor = color565(35, 142, 35); // green
	uint16_t devcolor = color565(205,173,0); // yellow
	uint16_t stacolor = color565(51,153,204); //blue
	fillScreen(BLACK);
	makeRectangle(10,10,90,140,WHITE, 2);
	makeRectangle(15,15,90,140,setcolor, 4);
	//makeRectangle(15,15,90,140,MAGENTA, 3);
	makeRectangle(10,160,90,140, WHITE, 2);
	makeRectangle(15,165,90,140,devcolor,4);
	//makeRectangle(15,165,90,140, BLACK, 3);
	makeRectangle(110,10, 90, 140, WHITE, 2);
	makeRectangle(115,15,90,140, schcolor,4);
	//makeRectangle(115,15, 90, 140, BLACK, 3);
	makeRectangle(110,160, 90, 140, WHITE, 2);
	makeRectangle(115,165, 90, 140, stacolor, 4);
	//makeRectangle(115,165, 90, 140, MAGENTA, 3);

	char * home = "HOME";
	char * set = "Settings";
	char * sched = "Schedule";
	char * dev = "Devices";
	char * stats = "Status";
	char buf[]= "00/00/00";
	twodigit(buf, GetMonth(),false);
	twodigit(buf+3, GetDate(), false);
	twodigit(buf+6, GetYear(), false);



	drawString(215,120 , home, WHITE, BLACK, 3);
	drawString(55,35, set, WHITE, BLACK, 2);
	drawString(155,35, dev, WHITE, BLACK, 2);
	drawString(55, 180, sched, WHITE, BLACK, 2);
	drawString(155,200, stats, WHITE, BLACK,  2);

	drawString(224, 1 , buf, WHITE, BLACK, 2);

    renderTime();
}
void HomeScreen::loop()
{
    uint8_t theTime = 4*GetHour() + GetMinute();
    if(theTime != displayedTime) {
        renderTime();
    }

    int x = getTouchX();
    int y = getTouchY();

    if (x > 10 && x<105 && y>10 && y < 155)
    {
        manager.presentScreen(settingsScreen);
    }
    else if (y> 160 && y<305 && x>10 && x<105)
    {
        manager.presentScreen(schedulePickDeviceScreen);
    }
    else if (x>110 && x<205 &&y>10 && y<155)
    {
        manager.presentScreen(deviceScreen);
    }
    else if (x>110 && x<205 && y>160 && y<305)
    {
        manager.presentScreen(statusScreen);
    }

    sleep(100);
}

StatusScreen::StatusScreen(){}
void StatusScreen::render()
{
    uint16_t stacolor = color565(51,153,204); //blue
    fillScreen(stacolor);
	makeRectangle(5,5, 225,310, BLACK, 4);
	char* status = "STATUS";
	drawString(225,50 , status, BLACK, stacolor, 2);

	makeRectangle(85,15, 135, 300, BLACK, 3);
	makeRectangle(15,15, 75, 300, BLACK, 3);
	drawString(210, 20, "Adapters", BLACK, stacolor, 2);
	drawString(85, 20, "Sensors ", BLACK, stacolor, 2);

	makeRectangle(100,50, 50,45, BLUE, 3);
	makeRectangle(100,95, 50,45, BLUE, 3);
	makeRectangle(100,180, 50,45, BLUE, 3);
	makeRectangle(100,225, 50,45, BLUE, 3);
	makeRectangle(155,50, 50,45, BLUE, 3);
	makeRectangle(155,95, 50,45, BLUE, 3);
	makeRectangle(155,180, 50,45, BLUE, 3);
	makeRectangle(155,225, 50,45, BLUE, 3);

	drawString(120, 58, "-3-", BLACK, stacolor,2);
	drawString(120, 188, "-4-", BLACK, stacolor, 2);
	drawString(178, 58, "-1-", BLACK, stacolor, 2);
	drawString(178, 188, "-2-", BLACK, stacolor, 2);
	drawString(120, 113, "ON", BLACK, stacolor,2);
	drawString(120, 233, "OFF", BLACK, stacolor, 2);
	drawString(178 , 102, "AUT", BLACK, stacolor, 2);
	drawString(178, 233, "OFF", BLACK, stacolor, 2);

	drawString(20, 30 , "2. ", BLACK, stacolor, 2);
	drawString(50, 30, "1. ", BLACK, stacolor, 2);
	drawString(20, 55, "ENABLED", BLACK, stacolor, 2);
	drawString(50, 55, "DISABLED", BLACK, stacolor, 2);
}
void StatusScreen::loop()
{
 // no links
}

DeviceScreen::DeviceScreen(){}
void DeviceScreen::render()
{
    uint16_t schcolor = color565(142,35,35);
	fillScreen(schcolor);
	makeRectangle(5,5, 225,310, BLACK, 4);
	drawPgmString(225,50 , PSTR("DEVICES"), BLACK, schcolor, 2);

	makeRectangle(60,60, 120, 200, BLACK, 7);
	makeRectangle(80, 30, 70, 120, BLACK, 5);
	makeRectangle(80, 160, 70,120, BLACK, 5);
	drawPgmString(115, 80, PSTR("ADD"), BLACK, schcolor, 2);
	drawPgmString(115 , 175, PSTR("REMOVE"), BLACK, schcolor, 2);
}
void DeviceScreen::loop()
{
	int x = getTouchX();
    int y = getTouchY();

	if (x>80 && x<150)
	{
		if (y>30 && y<150)
		{
			manager.presentScreen(deviceAddScreen);
		}
		// else if (y>160 && y<280)
		// {

		// }

	}

}

DeviceRemoveScreen::DeviceRemoveScreen(){}
void DeviceRemoveScreen::render()
{
    uint16_t schcolor = color565(142,35,35);
    fillScreen(schcolor);
    makeRectangle(15,15, 200,280, BLACK, 5);
    drawPgmString(210,50 , PSTR("REMOVE ADAPTERS"), BLACK, schcolor, 2);
}
void DeviceRemoveScreen::loop()
{

}

DeviceAddScreen::DeviceAddScreen(){}
void DeviceAddScreen::render()
{
    uint16_t schcolor = color565(142,35,35);
    fillScreen(BLACK);
    makeRectangle(15,15, 200,280, BLACK, 5);
    drawPgmString(210,50 , PSTR("ADD ADAPTERS"), schcolor, BLACK, 2);
    drawPgmString(180,50, PSTR("While this screen is open, devices will"), WHITE, BLACK, 1);
    drawPgmString(170,50, PSTR("synchronize with the basestation. Press"), WHITE, BLACK, 1);
    drawPgmString(160,50, PSTR("and hold HOME or BACK to exit."), WHITE, BLACK, 1);

    drawCircle(100, 100, 30, WHITE);
    drawCircle(100, 220, 30, WHITE);
    drawHorizontalLine(100, 130, 60, WHITE, 1);
    // 100, 190 is point of arrow
    for(int8_t i=0; i < 8; i++) {
        drawPixel(100-i, 190-i, WHITE);
        drawPixel(100+i, 190-i, WHITE);
    }

    for(int8_t i=0; i < 8; i++) {
        drawPixel(100-i, 130+i, WHITE);
        drawPixel(100+i, 130+i, WHITE);
    }
}
void DeviceAddScreen::afterRender()
{
    network.set_mode(LGNETWORK_DISCOVER);
}
void DeviceAddScreen::loop()
{
    network.loop();
}
void DeviceAddScreen::beforeExit()
{
    network.set_mode(LGNETWORK_OPERATE);
}

SettingsScreen::SettingsScreen(){}
void SettingsScreen::render()
{
    //rendering //
    uint16_t setcolor = color565(35,142,35);
	fillScreen(setcolor);
	makeRectangle(5,5, 225,310, BLACK, 4);
	drawString(225,50 , "SETTINGS", BLACK, setcolor, 2);

	makeRectangle(60, 30, 130, 125, BLACK, 4);
	makeRectangle(60, 170, 130, 125, BLACK, 4);
	makeRectangle(50, 20, 130, 125, BLACK, 4);
	makeRectangle(50, 180, 130, 125, BLACK, 4);
	drawString(90, 50, ("MODE"), BLACK , setcolor, 3);
    drawPgmString(90, 200, PSTR("TIME"), BLACK, setcolor, 3);
	drawPgmString(130, 50, PSTR("SET"), BLACK, setcolor, 3);
	drawPgmString(130, 200, PSTR("SET"), BLACK, setcolor, 3);
}
void SettingsScreen::loop()
{
    int x = getTouchX();
    int y = getTouchY();

    if ((y > 30 && y<155) && (x> 60 && x<190)){
        // SET MODE //
        manager.presentScreen(settingsSetModeScreen);
    }

	if ((y > 170 && y<295) && (x> 60 && x< 190)){
        // SET TIME //
        manager.presentScreen(settingsSetTimeScreen);
    }

    sleep(100);
}

SettingsSetTimeScreen::SettingsSetTimeScreen(){}
void SettingsSetTimeScreen::render()
{
    uint16_t setcolor = color565(35,142,35);
    fillScreen(BLACK);
	makeRectangle(5,5, 230,310, setcolor, 3);
	drawPgmString(225,50 , PSTR("SET TIME"), setcolor, BLACK, 2);

	drawVerticalLine(0,160,235, setcolor,  3);

	makeRectangle(190, 30, 30,100, WHITE, 3);
	makeRectangle(190, 190, 30, 100, WHITE, 3);
	drawPgmString(195, 60, PSTR("DATE"), WHITE, BLACK, 2);
	drawPgmString(195, 220, PSTR("TIME"), WHITE , BLACK, 2);

	drawPgmString(175, 10, PSTR("MONTH"), WHITE, BLACK, 1);
	drawPgmString(175, 63, PSTR("DAY"), WHITE, BLACK, 1);
	drawPgmString(175, 107, PSTR("YEAR"), WHITE, BLACK, 1);
	drawPgmString(175, 165, PSTR("HOUR"), WHITE, BLACK, 1);
	drawPgmString(175, 213, PSTR("MINUTE"), WHITE, BLACK, 1);
	drawPgmString(175, 276, PSTR("AM/PM"), WHITE, BLACK, 1);

	makeRectangle(135, 10, 35, 33, BLUE, 3);
	makeRectangle(135, 63, 35, 34, BLUE, 3);
	makeRectangle(135, 117, 35, 33, BLUE, 3);
	makeRectangle(135, 170, 35, 33, BLUE, 3);
	makeRectangle(135, 223, 35, 34, BLUE, 3);
	makeRectangle(135, 277, 35, 33, BLUE, 3);

	drawChar(110, 45, '/', WHITE, BLACK, 2);
	drawChar(110, 95, '/', WHITE, BLACK, 2);
	drawChar(110, 205, ':', WHITE, BLACK, 2);

	drawHorizontalLine(105, 0, 319, WHITE, 3);
	drawHorizontalLine(125, 0, 319, WHITE, 3);
	makeRectangle(60, 10, 35, 33, RED, 3);
	makeRectangle(60, 63, 35, 34, RED, 3);
	makeRectangle(60, 117, 35, 33, RED, 3);
	makeRectangle(60, 170, 35, 33, RED, 3);
	makeRectangle(60, 223, 35, 34, RED, 3);
	makeRectangle(60, 277, 35, 33, RED, 3);

	drawChar(150, 20,  '+', BLUE, BLACK, 2);
	drawChar(150, 73,  '+', BLUE, BLACK, 2);
	drawChar(150, 127, '+', BLUE, BLACK, 2);
	drawChar(150, 180, '+', BLUE, BLACK, 2);
	drawChar(150, 233, '+', BLUE, BLACK, 2);
	drawChar(150, 287, '+', BLUE, BLACK, 2);

	drawChar(75, 20,  '-', RED, BLACK, 2);
	drawChar(75, 73,  '-', RED, BLACK, 2);
	drawChar(75, 127, '-', RED, BLACK, 2);
	drawChar(75, 180, '-', RED, BLACK, 2);
	drawChar(75, 233, '-', RED, BLACK, 2);
	drawChar(75, 287, '-', RED, BLACK, 2);

	// makeRectangle(10, 20, 20, 120, GREEN, 3);
	// drawString(12, 30, "CONFIRM", BLACK, WHITE, 2);
    renderMonth();
    renderDay();
    renderYear();
    renderHour();
    renderMinute();
    renderAMPM();
}
void SettingsSetTimeScreen::renderMonth()
{
    // numToStr(GetMonth());
	twodigit(buf, GetMonth());
    drawString(110, 15, buf, WHITE, BLACK, 2);
}
void SettingsSetTimeScreen::renderDay()
{
    // numToStr(GetDate());
	twodigit(buf, GetDate());
    drawString(110, 68, buf, WHITE, BLACK, 2);
}
void SettingsSetTimeScreen::renderYear()
{
    // numToStr(GetYear());
	twodigit(buf, GetYear());
    drawString(110, 122, buf, WHITE, BLACK, 2);
}
void SettingsSetTimeScreen::renderHour()
{
    // numToStr(GetHour());
	twodigit(buf, GetHour());
    drawString(110, 175, buf, WHITE, BLACK, 2);
}
void SettingsSetTimeScreen::renderMinute()
{
    // numToStr(GetMinute());
	twodigit(buf, GetMinute());
    drawString(110, 228, buf, WHITE, BLACK, 2);
}
void SettingsSetTimeScreen::renderAMPM()
{
    if(GetAmPm())
        drawPgmString(110, 282, PSTR("PM"), WHITE, BLACK, 2);
    else
        drawPgmString(110, 282, PSTR("AM"), WHITE, BLACK, 2);
}
void SettingsSetTimeScreen::loop()
{
    return;
    int x = getTouchX();
    int y = getTouchY();

    // TODO
    if (x>54 && x<88) {
        //-
        if (y>6 && y<27) {
            // month
            uint8_t month = GetMonth();
            uint8_t next_month = month - 1;
            if(month == 0)
                next_month = 12;

            SetMonth(next_month);
            renderMonth();
        }
        if (y>40 && y<61) {
            // day
            uint8_t day = GetDate();
            uint8_t next_day = day - 1;
            if(day == 0)
                next_day = 31;

            SetMonth(next_day);
            renderDay();
        }
        if (y>73 && y<94) {
            // yr
            uint8_t year = GetYear();
            uint8_t next_year = year - 1;
            SetYear(next_year);
            renderYear();
        }
        if (y>106 && y<127) {
            // hr
            uint8_t hour = GetHour();
            uint8_t next_hour = hour - 1;
            SetHour(next_hour);
            renderHour();
        }
        if (y>140 && y<161) {
            // min
            uint8_t minute = GetMinute();
            uint8_t next_minute = minute - 1;
            SetMinute(next_minute);
            renderMinute();
        }
        if (y>173 && y<194) {
            // am/pm
            uint8_t ampm = GetAmPm();
            SetAmPm(~ampm);
        }

    }
    if (x>104 && x<138) {
        //+
        if (y>6 && y<27) {
            // month
        }
        if (y>40 && y<61) {
            // day
        }
        if (y>73 && y<94) {
            // yr
        }
        if (y>106 && y<127) {
            // hr
        }
        if (y>140 && y<161) {
            // min
        }
        if (y>173 && y<194) {
            // am/pm
        }
    }
}

SettingsSetModeScreen::SettingsSetModeScreen(){}
void SettingsSetModeScreen::render()
{
    uint16_t setcolor = color565(35,142,35);
    fillScreen(BLACK);
	makeRectangle(5,5, 230,310, setcolor, 2);
	drawPgmString(225,50 , PSTR("SET MODE"), setcolor, BLACK, 2);

	makeRectangle(35, 20, 35, 280, WHITE, 3);
	makeRectangle(85, 20, 35, 280, WHITE, 3);
	makeRectangle(135, 20, 35, 280, WHITE, 3);
	makeRectangle(185, 20, 35, 280, WHITE, 3);

	//drawVerticalLine(0,50, 239, BLACK, 4);
	drawVerticalLine(0, 270, 239, BLACK, 4);

	drawPgmString(200, 55, PSTR("1. SCHEDULE"), WHITE, BLACK, 2);
	drawPgmString(150, 55, PSTR("2. MOTION"), WHITE, BLACK, 2);
	drawPgmString(100, 55, PSTR("3. COMBINATION"), WHITE, BLACK, 2);
	drawPgmString(50, 55,  PSTR("4. VACATION"), WHITE, BLACK, 2);

	// makeRectangle(10, 60, 20, 160, GREEN, 3);
	// drawString( 15, 100, "CONFIRM", setcolor, WHITE, 2);
}
void SettingsSetModeScreen::loop()
{
    int x = getTouchX();
    int y = getTouchY();

    // touchscreen //
	// y uniform//
	if (y>169 && y<194);
	if (x> 33 && x<63);  // schedule mode //
	if (x>75 && x<105);  // motion only//
	if (x>117 && x< 147);  //both//
	if (x>159 && x<189);  // vacation //
}

static uint8_t scheduleScreenCurrentDay;
ScheduleScreen::ScheduleScreen() : consecutive_presses(0) {}
void ScheduleScreen::renderDays()
{
    PGM_P pstr = PSTR("SuMoTuWeThFrSa");
    char dstr[3];
    dstr[2] = 0; // null terminate string buf

    uint16_t color;

    for(uint8_t i=0; i < 7; i++) {
        // Get string part
        dstr[0] = pgm_read_byte(pstr + i*2);
        dstr[1] = pgm_read_byte(pstr + i*2 + 1);

        // Get color
        if(scheduleScreenCurrentDay == i)
            color = MAGENTA;
        else
            color = WHITE;

        drawString(200, 21 + 43*i, dstr, color, BLACK, 2);
    }
}
void ScheduleScreen::renderOnTime()
{
    char buf[3];

    uint8_t hr = LGDB::read_hour(device_idx, scheduleScreenCurrentDay, true);

    if(hr >= 12)
        drawString(140, 220, "PM", WHITE, BLACK, 2);
    else
        drawString(140, 220, "AM", WHITE, BLACK, 2);

    hr = hr % 12;
    if(hr == 0)
        hr = 12;
    twodigit(buf, hr);
    drawString(140, 136, buf, WHITE, BLACK, 2);

    drawChar(140, 170, ':', WHITE, BLACK, 2);

    // Minute
    uint8_t min = LGDB::read_minute(device_idx, scheduleScreenCurrentDay, true);
    twodigit(buf, min);
    drawString(140, 190, buf, WHITE, BLACK, 2);



}
void ScheduleScreen::renderOffTime()
{
    char buf[3];

    uint8_t hr = LGDB::read_hour(device_idx, scheduleScreenCurrentDay, false);

    if(hr >=12 )
        drawString(85, 220, "PM", WHITE, BLACK, 2);
    else
        drawString(85, 220, "AM", WHITE, BLACK, 2);

    hr = hr % 12;
    if(hr == 0)
        hr = 12;
    twodigit(buf, hr);
    drawString(85, 136, buf, WHITE, BLACK, 2);

    drawChar(85, 170, ':', WHITE, BLACK, 2);

    uint8_t min = LGDB::read_minute(device_idx, scheduleScreenCurrentDay, false);
    twodigit(buf, min);
    drawString(85, 190, buf, WHITE, BLACK, 2);



}

void ScheduleScreen::renderAuto()
{

    uint8_t sensor_idx = LGDB::read_sensor_table_entry(device_idx, scheduleScreenCurrentDay);
    if(sensor_idx == 0xFF) {
        drawString(35, 136, "No Sensor", WHITE, BLACK, 2);
    } else {
        char str[] = "Sensor 00";
        twodigit(str+7, sensor_idx);
        drawString(35, 136, str, WHITE, BLACK, 2);
    }

}
void ScheduleScreen::render()
{
    fillScreen(BLACK);
    uint16_t devcolor = color565(205,173,0);
	makeRectangle(5,5, 230,310, devcolor, 2);

	char* Adapter = "Adapter:00";
    twodigit(Adapter+8, device_idx, false);

	drawString(225,50 , Adapter, WHITE  , BLACK, 2);

	makeRectangle(180,6,40,44, WHITE, 3);
	makeRectangle(180,50,40,44, WHITE, 3);
	makeRectangle(180,94,40,44, WHITE, 3);
	makeRectangle(180,138,40,44, WHITE, 3);
	makeRectangle(180,182,40,44, WHITE, 3);
	makeRectangle(180,226,40,44, WHITE, 3);
	makeRectangle(180,270,40,44, WHITE, 3);

    renderDays();


	drawString(140, 25, "ON", WHITE, BLACK, 3);
	drawString(85, 15, "OFF", WHITE, BLACK, 3);
	drawString(30, 10, "AUTO", WHITE, BLACK, 3);


	makeRectangle(130,85, 40, 40, WHITE, 2); //on-
	makeRectangle(75, 85, 40, 40, WHITE, 2); //off-
	makeRectangle(20, 85, 40, 40, WHITE, 2); // auto-

	makeRectangle(130,255, 40,40, WHITE, 2); //on+
	makeRectangle(75, 255, 40, 40, WHITE, 2); //off+
	makeRectangle(20, 255, 40, 40, WHITE, 2); // auto+

	drawChar(140, 100, '-', RED, BLACK, 2);
	drawChar(85, 100, '-', RED, BLACK, 2);
	drawChar(35, 100, 'x', RED, BLACK, 2);
	drawChar(140, 270, '+', BLUE, BLACK, 2);
	drawChar(85, 270, '+', BLUE, BLACK, 2);
	drawChar(30, 270, '+', BLUE, BLACK, 2);

	//strings to be updated with touch dimensions //

    renderOffTime();
    renderOnTime();
	renderAuto();

	// makeRectangle(40, 40, 40, 160, BLACK, 3);
	// drawString(50, 50, "CONFIRM", BLACK, WHITE, 3);
}
void ScheduleScreen::loop()
{
    int x = getTouchX();
    int y = getTouchY();

    // LGSerial::put("X"); LGSerial::print(x);
    // LGSerial::put("Y"); LGSerial::print(y);
    // LGSerial::print("----");

    // touch screen dimensions //
	if ((x<180 && x>140))
    {
        scheduleScreenCurrentDay = (y - 6)/44; // Starts at 6px, each is 44px wide
        renderDays();
        renderOnTime();
        renderOffTime();
        renderAuto();
    }

	//off+ //
    bool pressed = false;
	if ((x>35 && x<75)	 && (y>255 && y<295)) {
        pressed = true;
        incr_time(device_idx, scheduleScreenCurrentDay, false, consecutive_presses);
    }
	//on+ //
	if ((x>90 && x<130)	 && (y>255 && y<295)) {
        pressed = true;
        incr_time(device_idx, scheduleScreenCurrentDay, true, consecutive_presses);
    }
	//off- //
	if ((x>35 && x<75)	 && (y>85 && y<125))
    {
        pressed = true;
        decr_time(device_idx, scheduleScreenCurrentDay, false, consecutive_presses);
    }
	//on-//
	if ((x>90 && x<130)	 && (y>85 && y<125))
    {
        pressed = true;
        decr_time(device_idx, scheduleScreenCurrentDay, true, consecutive_presses);
    }
    if(pressed) {
        consecutive_presses++;

        renderOnTime();
        renderOffTime();
    } else {
        consecutive_presses = 0;
    }


	// auto- //
	if ((x>-20 && x<20)	 && (y>85 && y<125))
	{
        // No sensor found?
        LGDB::write_sensor_table_entry(device_idx, scheduleScreenCurrentDay, 0xFF);
        renderAuto();
	}
	// auto+ //
	if ((x>-20 && x<20)	 && (y>255 && y<295))
	{
        uint8_t currentValue = LGDB::read_sensor_table_entry(device_idx, scheduleScreenCurrentDay);
        uint8_t startingIdx;
        if(currentValue == 0xFF)
            startingIdx = 0;
        else
            startingIdx = currentValue+1;

        for(uint8_t i=startingIdx; i < sizeof(lgdb_sensor_table); i++) {
            uint8_t data = LGDB::read_device_table_entry(i);
            if(data == 1) {
                // Found a sensor
                LGDB::write_sensor_table_entry(device_idx, scheduleScreenCurrentDay, i);
                renderAuto();
                break;
            }
        }
        // Otherwise, we found no applicable sensors
	}
    sleep(100);
}
void ScheduleScreen::incr_time(uint8_t device, uint8_t day, bool on_off_b, uint8_t increment)
{
    uint16_t data = LGDB::read_schedule_table_entry(device, day);
    uint8_t hour = data;
    if(on_off_b)
        hour = data >> 8;

    hour &= 0x7F;
    hour += (increment) ? 4 : 1;
    hour = hour % 96;

    if(on_off_b){
        data &= 0x80FF;
        uint16_t mask = hour;
        mask = mask << 8;
        data |= mask;
    } else {
        data &= 0xFF80;
        data |= hour;
    }

    LGDB::write_schedule_table_entry(device, day, data);
}
void ScheduleScreen::decr_time(uint8_t device, uint8_t day, bool on_off_b, uint8_t increment)
{
    uint16_t data = LGDB::read_schedule_table_entry(device, day);
    int8_t hour = data;
    if(on_off_b)
        hour = data >> 8;

    hour &= 0x7F;
    hour -= (increment) ? 4 : 1;
    hour = hour % 96;

    if(on_off_b){
        data &= 0x80FF;
        uint16_t mask = hour;
        mask = mask << 8;
        data |= mask;
    } else {
        data &= 0xFF80;
        data |= hour;
    }

    LGDB::write_schedule_table_entry(device, day, data);
}
SchedulePickDeviceScreen::SchedulePickDeviceScreen(){}
void SchedulePickDeviceScreen::render()
{
    uint16_t devcolor = color565(205,173,0);
    fillScreen(devcolor);
    makeRectangle(5,5, 230,305, BLACK, 2);
	drawPgmString(225,50 , PSTR("PICK ADAPTER"), BLACK, devcolor, 2);

    uint8_t start_idx = 0;

    if(LGDB::read_device_table_entry(start_idx) == 0) {
        drawPgmString(163, 67, PSTR("-0-"), BLACK, devcolor, 3);
        makeRectangle(130,30,80,120, BLACK, 4);
    }

    if(LGDB::read_device_table_entry(start_idx + 1) == 0) {
        drawPgmString(163, 212, PSTR("-1-") , BLACK, devcolor, 3);
        makeRectangle(130,170,80,120, BLACK, 4);
    }

    if(LGDB::read_device_table_entry(start_idx + 2) == 0) {
        drawPgmString(63, 67, PSTR("-2-"), BLACK, devcolor, 3);
        makeRectangle(30,30,80,120,BLACK, 4);
    }

    if(LGDB::read_device_table_entry(start_idx + 3) == 0) {
        drawPgmString(63, 212, PSTR("-3-") , BLACK, devcolor, 3);
        makeRectangle(30,170,80,120, BLACK, 4);
    }

}
void SchedulePickDeviceScreen::loop()
{
    int x = getTouchX();
    int y = getTouchY();

	if( x> 110 && x<170 && y>30 && y<150 )
	{
        scheduleScreen.device_idx = 0;
        manager.presentScreen(scheduleScreen);
	}
	else if ( x>110 && x<170 && y> 170 && y<290)
	{
        scheduleScreen.device_idx = 1;
        manager.presentScreen(scheduleScreen);
	}
	else if ( x> 0 && x<50 && y>30 && y<150)
	{
        scheduleScreen.device_idx = 2;
        manager.presentScreen(scheduleScreen);
	}
	else if (x> 0 && x<50 && y>170 && y<290 )
	{
        scheduleScreen.device_idx = 3;
        manager.presentScreen(scheduleScreen);
	}

    sleep(100);
}

LGUIScreen::LGUIScreen(){}

int LGUIScreen::getTouchX()
{
    DDRC = 0x0;
    DDRC = 0x5;

    PORTC |= YM;

    ADMUX |= (1<<REFS0);
    ADMUX &= ~(1<<REFS1);

    ADMUX |= (1<<ADLAR);

    ADMUX |= (3 << MUX0);
    ADMUX &= (0xf0 | (3<<MUX0));

    ADCSRA |= (7<<ADPS0);
    ADCSRA |= (1<<ADEN);

    ADCSRA |= (1<<ADSC);

    while (ADCSRA& (1<<ADSC));
    int raw = (ADCH);
    return 240 - (raw * 12)/10;

}

int LGUIScreen::getTouchY()
{
    DDRC = 0x0;
    DDRC = 0xa;

    PORTC |= XP;

    ADMUX |= (1<<REFS0);
    ADMUX &= ~(1<<REFS1);

    ADMUX |= (1<<ADLAR);

    ADMUX |= (0 << MUX0);
    ADMUX &= (0xf0 | (0<<MUX0));

    ADCSRA |= (7<<ADPS0);
    ADCSRA |= (1<<ADEN);

    ADCSRA |= (1<<ADSC);

    while (ADCSRA& (1<<ADSC));
    // DDRC |= 0x5;
    int raw = (ADCH);
    return (raw * 16)/10 - 30;
}

