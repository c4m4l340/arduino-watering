#ifndef __USER_INTERFACE_H__
#define __USER_INTERFACE_H__

#include "Lcd.h"
#include "Keypad.h"
#include "WaterProgram.h"

#define USER_INTERFACE_STATUS_OFF 0
#define USER_INTERFACE_STATUS_STANDBY 1
#define USER_INTERFACE_STATUS_USING_MENU 2

class UserInterface{
    private:
        byte currentStatus = USER_INTERFACE_STATUS_OFF;
        //byte previousStatus = USER_INTERFACE_STATUS_OFF;

        Keys pressedKey = Keys::KEY_NO_KEY;

        Lcd* lcd;
        static void onLcdSleep(void* caller_ptr);
        static void onLcdWakeup(void* caller_ptr);
        
        Keypad* keypad;        
        static void onKeyUp(int key, void* caller_ptr);
        static void onKeyDown(int key, void* caller_ptr);

        WaterProgram* waterProg;

        void showIdleScreen(int hours, int minutes, int seconds);
        void processMenu(Keys pressedKey);

    public:
        UserInterface(Lcd* lcd, Keypad* keypad, WaterProgram* waterProgram);
        void begin();
        void update();

        void pushTime(int hours, int minutes, int seconds);
};

#endif
