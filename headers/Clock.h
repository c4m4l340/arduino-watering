#ifndef __CLOCK_H__
#define __CLOCK_H__

#include <Wire.h>
#include <DS3231.h>

using TimeGetEvent = void (*)(DateTime datetime);

class Clock{

    private:
        DS3231 ds3231;
        RTClib rtc;
        
        unsigned long previousTime = 0;

    public:
        TimeGetEvent onGetTime = NULL;

        Clock();
        void begin();
        void update();

};

#endif