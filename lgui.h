#include "adafruit335.h"
#include "lgnetwork.h"

#ifndef LGUI_H
#define LGUI_H

// For touch
#define XM (1 << PC0);
#define XP (1 << PC1);
#define YM (1 << PC2);
#define YP (1 << PC3);

class ScreenManager;

/**
* Base class for screens
*/
class LGUIScreen
{
public:
    LGUIScreen();
    virtual void render();
    virtual void loop();
    virtual void beforeRender() {};
    virtual void afterRender() {};
    virtual void beforeExit() {};

protected:
    int getTouchX();
    int getTouchY();
};

class HomeScreen : public LGUIScreen
{
public:
    HomeScreen();
    void render();
    void renderTime();
    void loop();

    uint8_t displayedTime;
};

class StatusScreen : public LGUIScreen
{
public:
    StatusScreen();
    void render();
    void loop();
};

class DeviceScreen : public LGUIScreen
{
public:
    DeviceScreen();
    void render();
    void loop();
};

class DeviceAddScreen : public LGUIScreen
{
public:
    DeviceAddScreen();
    void render();
    void loop();
    virtual void afterRender();
    virtual void beforeExit();
};

class DeviceRemoveScreen : public LGUIScreen
{
public:
    DeviceRemoveScreen();
    void render();
    void loop();
};

class SettingsScreen : public LGUIScreen
{
public:
    SettingsScreen();
    void render();
    void loop();
};

class SettingsSetTimeScreen : public LGUIScreen
{
public:
    SettingsSetTimeScreen();
    void render();
    void loop();

private:
    void renderMonth();
    void renderDay();
    void renderYear();
    void renderHour();
    void renderMinute();
    void renderAMPM();

    void numToStr(uint8_t num);
    char buf[3];
};

class SettingsSetModeScreen : public LGUIScreen
{
public:
    SettingsSetModeScreen();
    void render();
    void loop();
};


class ScheduleScreen : public LGUIScreen
{
public:
    ScheduleScreen();
    void render();
    void renderDays();
    void renderOnTime();
    void renderOffTime();
	void renderAuto();
    void loop();

    uint8_t device_idx;
private:
    uint8_t consecutive_presses;
    void incr_time(uint8_t device, uint8_t day, bool on_off_b, uint8_t increment);
    void decr_time(uint8_t device, uint8_t day, bool on_off_b, uint8_t increment);
};

class SchedulePickDeviceScreen : public LGUIScreen
{
public:
    SchedulePickDeviceScreen();
    void render();
    void loop();
};

class ScreenManager
{
public:
    ScreenManager();
    void presentScreen(LGUIScreen &s);
    void loop();

private:
    LGUIScreen *currentScreen;
    LGUIScreen* history[5];
    uint8_t history_idx;
};

extern ScreenManager manager;

extern HomeScreen homeScreen; // Links to Settings, Status, Devices, Schedule

extern StatusScreen statusScreen;

extern DeviceScreen deviceScreen;
extern DeviceAddScreen deviceAddScreen;
extern DeviceRemoveScreen deviceRemoveScreen;

extern SettingsScreen settingsScreen; // Links to Set Time, Set Mode
extern SettingsSetTimeScreen settingsSetTimeScreen;
extern SettingsSetModeScreen settingsSetModeScreen;

extern SchedulePickDeviceScreen schedulePickDeviceScreen; // Links to schedule
extern ScheduleScreen scheduleScreen;

#endif
