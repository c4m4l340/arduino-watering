#ifndef __DATE_TIME_SETTING__
#define __DATE_TIME_SETTING__

#include "MenuActionBase.h"
#include "Lcd.h"

#define DATETIMESETTING_YEAR 0
#define DATETIMESETTING_MONTH 1
#define DATETIMESETTING_DAY 2
#define DATETIMESETTING_HOUR 3
#define DATETIMESETTING_MINUTE 4

class DateTimeSetting: public MenuActionBase{

    private:
        Lcd* lcd;

        int year;
        int month;
        int day;
        int hour;
        int minute;
        
        bool setDate;
        bool setTime;

        int currentStatus;
        long lastBlinkUpdateTime;
        int blinkOn;

        void blink();
    public:

        void begin();
        void update();

        void keyUp();
        void keyDown();
        void keyEnter();
        void keyBack();

        DateTimeSetting(Lcd* lcd);

        void initDateTime();
        void initTime();
        void getSettedDateTime(int* year, int* month, int* day, int* hour, int* minute);
        void getSettedTime(int* hour, int* minute);
};

#endif