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

ScreenManager::ScreenManager() : currentScreen(NULL)
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
        //
    } else if( homeButton && (currentScreen != &homeScreen) ) {
        presentScreen(homeScreen);
    } else {
        if(currentScreen)
            currentScreen->loop();
    }
}

HomeScreen::HomeScreen(){}
void HomeScreen::render()
{
    uint16_t schcolor = color565(142,35,35); //red
    uint16_t setcolor = color565(35, 142, 35); // green
	uint16_t devcolor = color565(205,173,0); // yellow
	uint16_t stacolor = color565(51,153,204); //blue
	fillScreen(WHITE);
	makeRectangle(10,10,90,140,BLACK, 4);
	makeRectangle(15,15,90,140,setcolor, 4);
	//makeRectangle(15,15,90,140,MAGENTA, 3);
	makeRectangle(10,160,90,140, BLACK, 4);
	makeRectangle(15,165,90,140,devcolor,4);
	//makeRectangle(15,165,90,140, BLACK, 3);
	makeRectangle(110,10, 90, 140, BLACK, 4);
	makeRectangle(115,15,90,140, schcolor,4);
	//makeRectangle(115,15, 90, 140, BLACK, 3);
	makeRectangle(110,160, 90, 140, BLACK, 4);
	makeRectangle(115,165, 90, 140, stacolor, 4);
	//makeRectangle(115,165, 90, 140, MAGENTA, 3);

	char * home = "HOME";
	char * set = "Settings";
	char * sched = "Schedule";
	char * dev = "Devices";
	char * stats = "Status";
	char buf[]= "00/00/00";
	twodigit(buf, GetMonth(),false); 
	twodigit(buf+3, GetDay(), false); 
	twodigit(buf+6, GetYear(), false); 
	
	

	drawString(215,120 , home, BLACK, WHITE, 3);
	drawString(55,35, set, BLACK, WHITE, 2);
	drawString(155,35, dev, BLACK, WHITE, 2);
	drawString(55, 180, sched, BLACK, WHITE, 2);
	drawString(155,200, stats, BLACK, WHITE,  2);

	drawString(224, 1 , buf, BLACK, WHITE, 2);
	makeRectangle(223,0, 15,100, RED, 2);
	makeRectangle(224, 219, 15, 100, RED, 2);
    /*
    unsigned int minutes = 0;
    unsigned int secs = 0;
    const char* secdisp = "0";
    const char* mindisp = "0";
    const char t = "";*/
	 
	//twodigit(buf, GetHour()); 
	char tim[] = "00:00 PM"; 
	twodigit(tim, GetHour(), false); 
	twodigit(tim+3, GetMinute(), false); 
	if (GetAmPm() == 0)
	{
		tim[6] = 'A'; 
	}
	

	
    drawString(225, 220, tim, BLACK, WHITE, 2);
    int x,y;
}
void HomeScreen::loop()
{
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

}

DeviceRemoveScreen::DeviceRemoveScreen(){}
void DeviceRemoveScreen::render()
{
    uint16_t schcolor = color565(142,35,35);
    fillScreen(schcolor);
    makeRectangle(15,15, 200,280, BLACK, 5);
    drawPgmString(210,50 , PSTR("ADD ADAPTERS"), BLACK, schcolor, 2);
}
void DeviceRemoveScreen::loop()
{

}

DeviceAddScreen::DeviceAddScreen(){}
void DeviceAddScreen::render()
{
    uint16_t schcolor = color565(142,35,35);
    fillScreen(schcolor);
    makeRectangle(15,15, 200,280, BLACK, 5);
    drawPgmString(210,50 , PSTR("ADD ADAPTERS"), BLACK, schcolor, 2);
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
	drawPgmString(225,50 , PSTR("SETTINGS"), BLACK, setcolor, 2);

	makeRectangle(60, 30, 130, 125, BLACK, 7);
	makeRectangle(60, 170, 130, 125, BLACK, 7);
	drawPgmString(90, 50, PSTR("MODE"), BLACK , setcolor, 3);
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
    fillScreen(WHITE);
	makeRectangle(5,5, 230,310, setcolor, 3);
	drawPgmString(225,50 , PSTR("SET TIME"), setcolor, WHITE, 2);

	drawVerticalLine(0,160,235, setcolor,  3);

	makeRectangle(190, 30, 30,100, BLACK, 3);
	makeRectangle(190, 190, 30, 100, BLACK, 3);
	drawPgmString(195, 60, PSTR("DATE"), BLACK, WHITE, 2);
	drawPgmString(195, 220, PSTR("TIME"), BLACK , WHITE, 2);

	drawPgmString(175, 10, PSTR("MONTH"), BLACK, WHITE, 1);
	drawPgmString(175, 63, PSTR("DAY"), BLACK, WHITE, 1);
	drawPgmString(175, 107, PSTR("YEAR"), BLACK, WHITE, 1);
	drawPgmString(175, 165, PSTR("HOUR"), BLACK, WHITE, 1);
	drawPgmString(175, 213, PSTR("MINUTE"), BLACK, WHITE, 1);
	drawPgmString(175, 276, PSTR("AM/PM"), BLACK, WHITE, 1);

	makeRectangle(135, 10, 35, 33, BLUE, 3);
	makeRectangle(135, 63, 35, 34, BLUE, 3);
	makeRectangle(135, 117, 35, 33, BLUE, 3);
	makeRectangle(135, 170, 35, 33, BLUE, 3);
	makeRectangle(135, 223, 35, 34, BLUE, 3);
	makeRectangle(135, 277, 35, 33, BLUE, 3);

	drawChar(110, 45, '/', BLACK, WHITE, 2);
	drawChar(110, 95, '/', BLACK, WHITE, 2);
	drawChar(110, 205, ':', BLACK, WHITE, 2);

	drawHorizontalLine(105, 0, 319, BLACK, 3);
	drawHorizontalLine(125, 0, 319, BLACK, 3);
	makeRectangle(60, 10, 35, 33, RED, 3);
	makeRectangle(60, 63, 35, 34, RED, 3);
	makeRectangle(60, 117, 35, 33, RED, 3);
	makeRectangle(60, 170, 35, 33, RED, 3);
	makeRectangle(60, 223, 35, 34, RED, 3);
	makeRectangle(60, 277, 35, 33, RED, 3);

	drawChar(150, 20,  '+', BLUE, WHITE, 2);
	drawChar(150, 73,  '+', BLUE, WHITE, 2);
	drawChar(150, 127, '+', BLUE, WHITE, 2);
	drawChar(150, 180, '+', BLUE, WHITE, 2);
	drawChar(150, 233, '+', BLUE, WHITE, 2);
	drawChar(150, 287, '+', BLUE, WHITE, 2);

	drawChar(75, 20,  '-', RED, WHITE, 2);
	drawChar(75, 73,  '-', RED, WHITE, 2);
	drawChar(75, 127, '-', RED, WHITE, 2);
	drawChar(75, 180, '-', RED, WHITE, 2);
	drawChar(75, 233, '-', RED, WHITE, 2);
	drawChar(75, 287, '-', RED, WHITE, 2);

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
    drawString(110, 15, buf, BLACK, WHITE, 2);
}
void SettingsSetTimeScreen::renderDay()
{
    // numToStr(GetDay());
	twodigit(buf, GetDay()); 
    drawString(110, 68, buf, BLACK, WHITE, 2);
}
void SettingsSetTimeScreen::renderYear()
{
    // numToStr(GetYear());
	twodigit(buf, GetYear()); 
    drawString(110, 122, buf, BLACK, WHITE, 2);
}
void SettingsSetTimeScreen::renderHour()
{
    // numToStr(GetHour());
	twodigit(buf, GetHour()); 
    drawString(110, 175, buf, BLACK, WHITE, 2);
}
void SettingsSetTimeScreen::renderMinute()
{
    // numToStr(GetMinute());
	twodigit(buf, GetMinute()); 
    drawString(110, 228, buf, BLACK, WHITE, 2);
}
void SettingsSetTimeScreen::renderAMPM()
{
    if(GetAmPm())
        drawPgmString(110, 282, PSTR("PM"), BLACK, WHITE, 2);
    else
        drawPgmString(110, 282, PSTR("AM"), BLACK, WHITE, 2);
}
void SettingsSetTimeScreen::loop()
{
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
            uint8_t day = GetDay();
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
    fillScreen(WHITE);
	makeRectangle(5,5, 230,310, setcolor, 2);
	drawPgmString(225,50 , PSTR("SET MODE"), setcolor, WHITE, 2);

	makeRectangle(35, 20, 35, 280, BLACK, 3);
	makeRectangle(85, 20, 35, 280, BLACK, 3);
	makeRectangle(135, 20, 35, 280, BLACK, 3);
	makeRectangle(185, 20, 35, 280, BLACK, 3);

	//drawVerticalLine(0,50, 239, BLACK, 4);
	drawVerticalLine(0, 270, 239, BLACK, 4);

	drawPgmString(200, 55, PSTR("1. SCHEDULE"), BLACK, WHITE, 2);
	drawPgmString(150, 55, PSTR("2. MOTION"), BLACK, WHITE, 2);
	drawPgmString(100, 55, PSTR("3. COMBINATION"), BLACK, WHITE, 2);
	drawPgmString(50, 55,  PSTR("4. VACATION"), BLACK, WHITE, 2);

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
ScheduleScreen::ScheduleScreen() {}
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
            color = BLACK;

        drawString(200, 21 + 43*i, dstr, color, WHITE, 2);
    }
}
void ScheduleScreen::renderOnTime()
{
    uint16_t data = LGDB::read_schedule_table_entry(device_idx, scheduleScreenCurrentDay);
    uint8_t ontime = (data & 0x7F00) >> 8;

    char buf[3];
    LGSerial::print(ontime);

    uint8_t hr = ((ontime % 44) / 4);
    twodigit(buf, hr);
    drawString(150, 136, buf, BLACK, WHITE, 2);
    drawChar(150, 170, ':', BLACK, WHITE, 2);

    // Minute
    uint8_t min = (ontime & 0x03) * 15;
    twodigit(buf, min);
    drawString(150, 190, buf, BLACK, WHITE, 2);

    if(ontime >= 48)
        drawString(150, 220, "PM", BLACK, WHITE, 2);
    else
        drawString(150, 220, "AM", BLACK, WHITE, 2);

}
void ScheduleScreen::renderOffTime()
{
    drawString(110, 136, "10", BLACK, WHITE, 2);
    drawChar(110, 170, ':', BLACK, WHITE, 2);
    drawString(110, 190, "30", BLACK, WHITE, 2);
    drawString(110, 220, "AM", BLACK, WHITE, 2);

}
void ScheduleScreen::render()
{
    fillScreen(WHITE);
    uint16_t devcolor = color565(205,173,0);
	makeRectangle(5,5, 230,310, devcolor, 2);

	char* Adapter = "Adapter:00";
    twodigit(Adapter+8, device_idx, false);

	drawString(225,50 , Adapter, BLACK  , WHITE, 2);

	makeRectangle(180,6,40,44, BLACK, 3);
	makeRectangle(180,50,40,44, BLACK, 3);
	makeRectangle(180,94,40,44, BLACK, 3);
	makeRectangle(180,138,40,44, BLACK, 3);
	makeRectangle(180,182,40,44, BLACK, 3);
	makeRectangle(180,226,40,44, BLACK, 3);
	makeRectangle(180,270,40,44, BLACK, 3);

    renderDays();

	drawString(150, 25, "ON", BLACK, WHITE, 3);
	drawString(110, 15, "OFF", BLACK, WHITE, 3);

	makeRectangle(140,74, 35, 40, BLACK, 2); //on-
	makeRectangle(100, 74, 35, 40, BLACK, 2); //off-

	makeRectangle(140,266, 35,40, BLACK, 2); //on+
	makeRectangle(100, 266, 35, 40, BLACK, 2); //off+

	drawChar(155, 85, '-', RED, WHITE, 2);
	drawChar(115, 85, '-', RED, WHITE, 2);
	drawChar(155, 275, '+', BLUE, WHITE, 2);
	drawChar(115, 275, '+', BLUE, WHITE, 2);
	//strings to be updated with touch dimensions //

    renderOffTime();
    renderOnTime();

	// makeRectangle(40, 40, 40, 160, BLACK, 3);
	// drawString(50, 50, "CONFIRM", BLACK, WHITE, 3);
}
void ScheduleScreen::loop()
{
    int x = getTouchX();
    int y = getTouchY();

    // touch screen dimensions //
	if ((x<180 && x>140))
    {
        scheduleScreenCurrentDay = (y - 6)/44; // Starts at 6px, each is 44px wide
        renderDays();
    }

	//off+ //
	if ((x>65 && x<100)	 && (y>226 && y<306)) {
        int8_t t = getOffTime();
        if(++t == 96) t = 0;
        setOffTime(t);

        renderOnTime();
        renderOffTime();
    }
	//on+ //
	if ((x>100 && x<135)	 && (y>266 && y<306)) {
        int8_t t = getOnTime();
        if(++t == 96) t = 0;
        setOnTime(t);

        renderOnTime();
        renderOffTime();
    }
	//off- //
	if ((x>65 && x<100)	 && (y>74 && y<114))
    {
        int8_t t = getOffTime();
        if(--t < 0) t = 95;
        setOffTime(t);

        renderOnTime();
        renderOffTime();
    }
	//on-//
	if ((x>100 && x<135)	 && (y>74 && y<114))
    {
        int8_t t = getOnTime();
        if(--t < 0) t = 95;
        setOnTime(t);

        renderOnTime();
        renderOffTime();
    }

    // sleep(20);
}
uint8_t ScheduleScreen::getOnTime()
{
    uint16_t data = LGDB::read_schedule_table_entry(device_idx, scheduleScreenCurrentDay);
    return (data & 0x7F00) >> 8;
}
uint8_t ScheduleScreen::getOffTime()
{
    uint16_t data = LGDB::read_schedule_table_entry(device_idx, scheduleScreenCurrentDay);
    return (data & 0x007F);
}

void ScheduleScreen::setOnTime(uint8_t time)
{
    uint16_t data = LGDB::read_schedule_table_entry(device_idx, scheduleScreenCurrentDay);
    data = data & 0xC0FF;
    data = data | (time << 8);
    LGDB::write_schedule_table_entry(device_idx, scheduleScreenCurrentDay, data);
}
void ScheduleScreen::setOffTime(uint8_t time)
{
    uint16_t data = LGDB::read_schedule_table_entry(device_idx, scheduleScreenCurrentDay);
    data = data & 0xFFC0;
    data = data | time;
    LGDB::write_schedule_table_entry(device_idx, scheduleScreenCurrentDay, data);
}

SchedulePickDeviceScreen::SchedulePickDeviceScreen(){}
void SchedulePickDeviceScreen::render()
{
    uint16_t devcolor = color565(205,173,0);
    fillScreen(devcolor);
    makeRectangle(5,5, 230,305, BLACK, 2);
	drawPgmString(225,50 , PSTR("PICK ADAPTER"), BLACK, devcolor, 2);

	makeRectangle(30,30,80,120,BLACK, 4);
	makeRectangle(130,30,80,120, BLACK, 4);
	makeRectangle(30,170,80,120, BLACK, 4);
	makeRectangle(130,170,80,120, BLACK, 4);

    uint8_t start_idx = 0;

    if(LGDB::read_device_table_entry(start_idx) != 0xFF) {
        drawPgmString(160, 60, PSTR("-0-"), BLACK, devcolor, 3);
    }

    if(LGDB::read_device_table_entry(start_idx + 1) != 0xFF) {
        drawPgmString(160, 205, PSTR("-1-") , BLACK, devcolor, 3);
    }

    if(LGDB::read_device_table_entry(start_idx + 2) != 0xFF) {
        drawPgmString(60, 60, PSTR("-2-"), BLACK, devcolor, 3);
    }

    if(LGDB::read_device_table_entry(start_idx + 3) != 0xFF) {
        drawPgmString(60, 205, PSTR("-3-") , BLACK, devcolor, 3);
    }

}
void SchedulePickDeviceScreen::loop()
{
    int x = getTouchX();
    int y = getTouchY();

	if( x>25 && x<92 && y>19 && y<94 )
	{
        scheduleScreen.device_idx = 0;
        manager.presentScreen(scheduleScreen);
	}
	else if ( x>108 && x<175 && y> 19 && y<94)
	{
        scheduleScreen.device_idx = 1;
        manager.presentScreen(scheduleScreen);
	}
	else if ( x>25 && x<92 && y>106 && y<181)
	{
        scheduleScreen.device_idx = 2;
        manager.presentScreen(scheduleScreen);
	}
	else if (x>108 && x<175 && y>106 && y<181 )
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

