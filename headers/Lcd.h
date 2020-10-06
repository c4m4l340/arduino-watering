#define DEBUG

#ifndef _LCD_H_
#define _LCD_H_

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LCD_LIGHT_TIMEOUT 10000

using SleepCallback = void (*)();
using WakeupCallback = void (*)();

class Lcd{
    private:
        LiquidCrystal_I2C lcd;

        unsigned long timerLightOn = 0;   

        bool isAwake;  

        void on();
        void off();

    public:
        SleepCallback onSleep = NULL;
        WakeupCallback onWakeup = NULL;

        Lcd(uint8_t address, uint8_t cols, uint8_t rows);
        void begin();
        void update();

        void wakeup();
};

#endif