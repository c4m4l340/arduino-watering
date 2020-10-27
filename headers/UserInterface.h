#ifndef __USER_INTERFACE_H__
#define __USER_INTERFACE_H__

#include "Lcd.h"
#include "Keypad.h"
#include "WaterProgram.h"
#include "Menu.h"
#include "MenuActionSetDateTime.h"

#define USER_INTERFACE_STATUS_OFF 0
#define USER_INTERFACE_STATUS_STANDBY 1
#define USER_INTERFACE_STATUS_USING_MENU 2
#define USER_INTERFACE_EXEC_ACTION 3

class UserInterface{
    private:
        byte currentStatus = USER_INTERFACE_STATUS_OFF;
        //byte previousStatus = USER_INTERFACE_STATUS_OFF;

        unsigned long lastDebugTime = 0;

        Keys pressedKey = Keys::KEY_NO_KEY;

        Lcd* lcd;
        static void onLcdSleep(void* caller_ptr);
        static void onLcdWakeup(void* caller_ptr);
        
        Keypad* keypad;        
        static void onKeyUp(int key, void* caller_ptr);
        static void onKeyDown(int key, void* caller_ptr);

        Menu* menu;
        //MenuItem* currentMenuItem = NULL;
        //MenuItem* previousMenuItem = NULL;
        static void execMenuOpen(MenuActionBase *menuAction, void* caller_ptr);
        static void execMenuClose(MenuActionBase *menuAction, void* caller_ptr);
        static void exeMenuAction(MenuActionBase* menuAction, void* caller_ptr);

        static void onActionComplete (void* data, void* caller_ptr);
        static void onActionCanceled (void* caller_ptr);

        WaterProgram* wprogram;
        MenuActionBase* menuAction;

        void showIdleScreen(int hours, int minutes, int seconds);
        void showMenuScreen(MenuItem* item);
        void processMenu(Keys pressedKey);
        void processAction(Keys pressedKey);
        void setMenuCallbacks();

    public:
        UserInterface(Lcd* lcd, Keypad* keypad, Menu* menu, WaterProgram* wprogram);
        void begin();
        void update();

        void pushTime(int hours, int minutes, int seconds);
};

#endif
