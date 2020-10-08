#ifndef __USER_INTERFACE_H__
#define __USER_INTERFACE_H__

#include "Lcd.h"
#include "Keypad.h"

#define USER_INTERFACE_STATUS_OFF 0
#define USER_INTERFACE_STANDBY 1
#define USER_INTERFACE_USING_MENU 2

class UserInterface{
    private:
        Lcd* lcd;
        static void onLcdSleep(void* caller_ptr);
        static void onLcdWakeup(void* caller_ptr);
        
        Keypad* keypad;        
        static void onKeyUp(int key, void* caller_ptr);
        static void onKeyDown(int key, void* caller_ptr);

    public:
        UserInterface(Lcd* lcd, Keypad* keypad);
        void begin();
        void update();
};

#endif
