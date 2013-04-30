#include "adafruit335.h"

#ifndef LGUI_H
#define LGUI_H

// For touch
#define XM (1 << PC0);
#define XP (1 << PC1);
#define YM (1 << PC2);
#define YP (1 << PC3);


/**
* Base class for screens
*/
class LGUIScreen
{
public:
    LGUIScreen();
    virtual void render();
    virtual void loop();

protected:
    int getTouchX();
    int getTouchY();
};

class HomeScreen : public LGUIScreen
{
public:
    HomeScreen();
    void render();
    void loop();
};

class StatusScreen : public LGUIScreen
{
public:
    StatusScreen();
    void render();
    void loop();
};

class AddDeviceScreen : public LGUIScreen
{
public:
    AddDeviceScreen();
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

class ScheduleScreen : public LGUIScreen
{
public:
    ScheduleScreen();
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
};

extern ScreenManager manager;
extern HomeScreen homeScreen;
extern StatusScreen statusScreen;
extern AddDeviceScreen addDeviceScreen;
extern SettingsScreen settingsScreen;
extern ScheduleScreen scheduleScreen;

#endif
