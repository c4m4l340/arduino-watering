#define DEBUG

#ifndef _LCD_H_
#define _LCD_H_

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WString.h>

#define LCD_LIGHT_TIMEOUT 10000

using SleepCallback = void (*)(void* caller_ptr);
using WakeupCallback = void (*)(void* caller_ptr);

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
        void* callerCallbackInstance = NULL;

        Lcd(uint8_t address, uint8_t cols, uint8_t rows);
        void begin();
        void update();

        void wakeup();

        void writeLn(byte row, byte col, char line[]);
        void clear();
};

#endif