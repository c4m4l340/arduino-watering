#define DEBUG

#include <Arduino.h>
#include "headers\UserInterface.h"
#include "headers\Lcd.h"
#include "headers\Keypad.h"
#include <WString.h>

UserInterface::UserInterface(Lcd* lcd, Keypad* keypad, WaterProgram* waterProgram){
    this->lcd = lcd;
    this->keypad = keypad;
    this->waterProg = waterProgram;
    
}

#pragma region Task setup and run
void UserInterface::begin(){
    this->currentStatus = USER_INTERFACE_STATUS_OFF;

    keypad->onKeyUp = onKeyUp;
    keypad->onKeyDown = onKeyDown;
    keypad->callerCallbackInstance = this;

    lcd->onSleep = onLcdSleep;
    lcd->onWakeup = onLcdWakeup;
    lcd->callerCallbackInstance = this;

    this->buildMenu();    
}

void UserInterface::update(){
    
    switch (this->currentStatus)
    {
        case USER_INTERFACE_STATUS_OFF:{
            
            break;
        }
        case USER_INTERFACE_STATUS_USING_MENU:{
            break;
        }

        case USER_INTERFACE_STATUS_STANDBY:{
            break;
        }
    }

}
#pragma endregion

#pragma region Actions

void UserInterface::pushTime(int hours, int minutes, int seconds){
     DPRINTLN_F("UserInterface::pushTime(%d,%d,%d):currentStatus=%d", hours, minutes, seconds, this->currentStatus);

    if(this->currentStatus == USER_INTERFACE_STATUS_STANDBY){
        this->showIdleScreen(hours, minutes, seconds);
    }
}

#pragma endregion

#pragma region Callbacks

void UserInterface::onKeyUp(int key, void* caller_ptr){
    DPRINTLN_F("UserInterface::onKeyUp(%d)", key);
    UserInterface* me = static_cast<UserInterface*>(caller_ptr);

    switch(me->currentStatus
    ){
        case USER_INTERFACE_STATUS_OFF:{
            me->lcd->wakeup();
            break;
        }  
        case USER_INTERFACE_STATUS_STANDBY:{
                if(key == Keys::KEY_ENTER){
                    me->processMenu(Keys::KEY_BACK); //reset
                    me->currentStatus = USER_INTERFACE_STATUS_USING_MENU;
                }
            break;
        } 
        case USER_INTERFACE_STATUS_USING_MENU:{
            me->processMenu(key);
            break;
        }
    }
}

void UserInterface::onKeyDown(int key, void* caller_ptr){
}

void UserInterface::onLcdSleep(void* caller_ptr){
        UserInterface* me = static_cast<UserInterface*>(caller_ptr);
        me->currentStatus = USER_INTERFACE_STATUS_OFF;
        DPRINTLN_F("UserInterface::onLcdSleep:currentStatus=%d",me->currentStatus);
        
        me->lcd->clear();
}

void UserInterface::onLcdWakeup(void* caller_ptr){
    UserInterface* me = static_cast<UserInterface*>(caller_ptr);
    me->currentStatus = USER_INTERFACE_STATUS_STANDBY;

     DPRINTLN_F("UserInterface::onLcdWakeup:currentStatus=%d",me->currentStatus);
}

#pragma endregion

#pragma region Private

void UserInterface::showIdleScreen(int hours, int minutes, int seconds){
    DPRINTLN_F("UserInterface::showIdleScreen(%d,%d,%d):currentStatus=%d", hours, minutes, seconds, this->currentStatus);    
    
    char line[8+1];
    sprintf(line, "%02d:%02d:%02d", hours, minutes, seconds);

    lcd->writeLn(1,8,line);
}

void UserInterface::processMenu(Keys pressedKey){
        switch (pressedKey)
        {
            case Keys::KEY_ENTER:
                menu->enter();
                break;
            case Keys::KEY_BACK:
                menu->reset();
            case Keys::KEY_UP:
                menu->moveNext();
                break;
            case Keys::KEY_DOWN:
                menu->movePrevious();
        };
    this->currentMenuItem = &(this->menu->getCurrentItem());
}

void UserInterface::buildMenu(){

    MenuItem root[] = {
        {NULL,"Run",0,NULL},
        {NULL, "Stop",0,NULL},
        {NULL, "Date/Time",0,NULL},
        {NULL, "Programs",0,NULL}
    };
    
    this->menu->setRoot(root[0]);
}

#pragma endregion