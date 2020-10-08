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

