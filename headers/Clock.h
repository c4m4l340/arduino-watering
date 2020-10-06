#ifndef __CLOCK_H__
#define __CLOCK_H__

#include <Wire.h>
#include <DS3231.h>

using TimeCheckedCallback = void (*)(DateTime datetime);

class Clock{

    private:
        DS3231 ds3231;
        RTClib rtc;
        
        unsigned long previousTime = 0;

    public:
        TimeCheckedCallback onGetTime = NULL;

        Clock();
        void begin();
        void update();

};

#endif