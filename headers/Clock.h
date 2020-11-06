#ifndef __CLOCK_H__
#define __CLOCK_H__

#include <Wire.h>
#include <DS3231.h>

using TimeCheckedCallback = void (*)(DateTime datetime, void* caller_ptr);

class Clock{

    private:
        DS3231 ds3231;
        RTClib rtc;
        
        unsigned long previousTime = 0;

    public:
        TimeCheckedCallback onGetTime = NULL;
        void* callerCallbackInstance = NULL;
        Clock();
        void begin();
        void update();

        void setDateTime(int year, int month, int day, int hour, int minute);

};

#endif