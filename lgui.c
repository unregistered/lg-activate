#include "lgui.h"

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

ScreenManager::ScreenManager()
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
    currentScreen->beforeExit();

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

    const char * home = "HOME";
    const char * set = "Settings";
    const char * sched = "Schedule";
    const char * dev = "Add Device";
    const char * stats = "Status";
    const char * day = "04/25/13";

    drawString(215,120 , home, BLACK, WHITE, 3);
    drawString(55,35, set, BLACK, setcolor, 2);
    drawString(155,20, dev, BLACK, schcolor, 2);
    drawString(55, 180, sched, BLACK, devcolor, 2);
    drawString(155,200, stats, BLACK, stacolor, 2);

    drawString(224, 1 , day, GREY, WHITE, 2);
    makeRectangle(223,0, 15,100, RED, 2);
    makeRectangle(224, 219, 15, 100, RED, 2);
    /*
    unsigned int minutes = 0;
    unsigned int secs = 0;
    const char* secdisp = "0";
    const char* mindisp = "0";
    const char t = "";*/
    const char * time = "01:23 PM";
    drawString(225, 220, time, GREY, WHITE, 2);
    int x,y;
}
void HomeScreen::loop()
{
    int x = getTouchX();
    int y = getTouchY();

    if (x > 30 && x<100 && y>60 && y < 130)
    {
        manager.presentScreen(deviceScreen);
    }
    else if (y> 140 && y<220 && x>30 && x<100)
    {
        manager.presentScreen(settingsScreen);
    }
    else if (x>125 && x<215 &&y>60 && y<130)
    {
        manager.presentScreen(statusScreen);
    }
    else if (x>125 && x<215 && y>140 && y<220)
    {
        manager.presentScreen(schedulePickDeviceScreen);
    }
}

StatusScreen::StatusScreen(){}
void StatusScreen::render()
{
    uint16_t stacolor = color565(51,153,204); //blue
    fillScreen(stacolor);
    makeRectangle(15,15, 200,280, BLACK, 5);
    const char* status = "STATUS";
    drawString(210,50 , status, BLACK, stacolor, 2);
}
void StatusScreen::loop()
{

}

DeviceScreen::DeviceScreen(){}
void DeviceScreen::render()
{
    uint16_t schcolor = color565(142,35,35);
    fillScreen(schcolor);
    makeRectangle(15,15, 200,280, BLACK, 5);
    drawString(210,50 , "ADD ADAPTERS", BLACK, schcolor, 2);
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
    drawString(210,50 , "ADD ADAPTERS", BLACK, schcolor, 2);
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
    drawString(210,50 , "ADD ADAPTERS", BLACK, schcolor, 2);
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
    uint16_t setcolor = color565(35,142,35);
    fillScreen(setcolor);
    makeRectangle(15,15, 200,280, BLACK, 5);
    const char* settings = "SETTINGS";
    drawString(210,50 ,settings, BLACK, setcolor, 2);
}
void SettingsScreen::loop()
{

}

SettingsSetTimeScreen::SettingsSetTimeScreen(){}
void SettingsSetTimeScreen::render()
{
    uint16_t setcolor = color565(35,142,35);
    fillScreen(setcolor);
    makeRectangle(15,15, 200,280, BLACK, 5);
    const char* settings = "SETTINGS";
    drawString(210,50 ,settings, BLACK, setcolor, 2);
}
void SettingsSetTimeScreen::loop()
{

}

SettingsSetModeScreen::SettingsSetModeScreen(){}
void SettingsSetModeScreen::render()
{
    uint16_t setcolor = color565(35,142,35);
    fillScreen(setcolor);
    makeRectangle(15,15, 200,280, BLACK, 5);
    const char* settings = "SETTINGS";
    drawString(210,50 ,settings, BLACK, setcolor, 2);
}
void SettingsSetModeScreen::loop()
{

}

ScheduleScreen::ScheduleScreen(){}
void ScheduleScreen::render()
{
    uint16_t devcolor = color565(205,173,0);
    fillScreen(devcolor);
    makeRectangle(15,15, 200,280, BLACK, 5);
    const char* Adapter = "PICK ADAPTER";
    drawString(210,50 , Adapter, BLACK, devcolor, 2);
}
void ScheduleScreen::loop()
{

}

SchedulePickDeviceScreen::SchedulePickDeviceScreen(){}
void SchedulePickDeviceScreen::render()
{
    uint16_t devcolor = color565(205,173,0);
    fillScreen(devcolor);
    makeRectangle(15,15, 200,280, BLACK, 5);
    const char* Adapter = "PICK ADAPTER";
    drawString(210,50 , Adapter, BLACK, devcolor, 2);
}
void SchedulePickDeviceScreen::loop()
{

}

LGUIScreen::LGUIScreen(){}

int LGUIScreen::getTouchX()
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
    return (ADCH);
}

int LGUIScreen::getTouchY()
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
    return (ADCH);
}

