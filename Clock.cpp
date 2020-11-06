#include <Wire.h>
#include <DS3231.h>
#include "headers\Clock.h"

Clock::Clock(){}

void Clock::begin(){
    Wire.begin();
    ds3231.setClockMode(false);
}

void Clock::update(){
    unsigned long currentTime = millis();

    if((currentTime - previousTime) > 1000){
        previousTime = currentTime;
        DateTime time = rtc.now();
        if(onGetTime != NULL){
            onGetTime(time, callerCallbackInstance);
        }
    }
}

void Clock::setDateTime(int year, int month, int day, int hour, int minute){
    ds3231.setYear(year - 2000);
    ds3231.setMonth(month);
    ds3231.setDate(day);
    ds3231.setHour(hour);
    ds3231.setMinute(minute);
    ds3231.setSecond(0);
}
