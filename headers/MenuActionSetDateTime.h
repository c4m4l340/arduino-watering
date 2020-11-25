#ifndef __ACTION_SET_DATETIME__
#define __ACTION_SET_DATETIME__

#include "MenuActionBase.h"
#include "Lcd.h"

#define DATETIMESETTING_YEAR 0
#define DATETIMESETTING_MONTH 1
#define DATETIMESETTING_DAY 2
#define DATETIMESETTING_HOUR 3
#define DATETIMESETTING_MINUTE 4
#define DATETIMESETTING_DURATION 5

struct MenuDateTime{
    int year;
    byte month;
    byte day;

    byte hour;
    byte minute;

    byte duration;
};

class MenuActionSetDateTime: public MenuActionBase{

    private:
        Lcd* lcd;

        MenuDateTime datetime = {0,0,0,0,0,0};

        bool setDateTime;
        bool setScheduler;

        byte currentStatus;
        long lastBlinkUpdateTime;
        bool blinkOn;

        void blink();
    public:

        void begin(Lcd* lcd);
        void update();

        void reset(byte tag);
        void keyUp();
        void keyDown();
        void keyEnter();
        void keyBack();

        MenuActionSetDateTime();

        void initSetDateTime();
        void initSetScheduler();
        void getSettedDateTime(MenuDateTime *dt);
        void getSettedScheduler(byte& hour, byte& minute, byte& duration);
};

#endif