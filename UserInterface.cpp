#define DEBUG

#include <Arduino.h>
#include "headers\UserInterface.h"
#include "headers\Lcd.h"
#include "headers\Keypad.h"
#include <WString.h>

UserInterface::UserInterface(Lcd* lcd, Keypad* keypad, WaterProgram* waterProgram, Menu* menu){
    this->lcd = lcd;
    this->keypad = keypad;
    this->waterProg = waterProgram;
    this->menu = menu;
    
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

    this->setupMenuCallbacks();    
}

void UserInterface::update(){
        if(millis() - lastDebugTime > 1000){
            DPRINTLN_F("UserInterface:currentStatus(%d):", this->currentStatus);
            lastDebugTime = millis();
        }


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
     //DPRINTLN_F("UserInterface::pushTime(%d,%d,%d):currentStatus=%d", hours, minutes, seconds, this->currentStatus);

    if(this->currentStatus == USER_INTERFACE_STATUS_STANDBY){
        this->showIdleScreen(hours, minutes, seconds);
    }
}

#pragma endregion

#pragma region Callbacks

void UserInterface::onKeyUp(int key, void* caller_ptr){
    DPRINTLN_F("UserInterface::onKeyUp(%d)", key);
    UserInterface* me = static_cast<UserInterface*>(caller_ptr);

    me->lcd->resetLightOnTimeout();
    switch(me->currentStatus)
    {
        case USER_INTERFACE_STATUS_OFF:{    
            me->lcd->wakeup();        
            break;
        }  
        case USER_INTERFACE_STATUS_STANDBY:{
                if(key == Keys::KEY_ENTER){
                    me->currentStatus = USER_INTERFACE_STATUS_USING_MENU;
                    me->processMenu(Keys::KEY_BACK); //reset                    
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
        DPRINTLN("UserInterface::onLcdSleep");
        
        me->lcd->clear();
}

void UserInterface::onLcdWakeup(void* caller_ptr){
    UserInterface* me = static_cast<UserInterface*>(caller_ptr);
    me->currentStatus = USER_INTERFACE_STATUS_STANDBY;

     DPRINTLN("UserInterface::onLcdWakeup");
}

#pragma endregion

#pragma region Private

void UserInterface::showIdleScreen(int hours, int minutes, int seconds){
    //DPRINTLN_F("UserInterface::showIdleScreen(%d,%d,%d):currentStatus=%d", hours, minutes, seconds, this->currentStatus);    
    
    char line[16+1] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    lcd->writeLn(1,0,"                \0");
    lcd->writeLn(0,0,"                \0");
    
    
    sprintf(line, "Good %s", (hours>6) && (hours<12)?"morning": ((hours>12)&&(hours<20)?"afternoon":"night" ));
    lcd->writeLn(0,0,line);

    sprintf(line, "%02d:%02d:%02d", hours, minutes, seconds);
    lcd->writeLn(1,0,line);
}

void UserInterface::showMenuScreen(MenuItem item){
    //DPRINTLN_F("UserInterface::showMenuScreen{%d,%s}:currentStatus=%d", item.title.c_str(), this->currentStatus);
    
    char line[16+1] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    lcd->writeLn(1,0,"                \0");
    lcd->writeLn(0,0,"                \0");

    snprintf(line,16+1,"%s%c",item.title.c_str(), (item.hasChildren()?0x7E:0x00));
    lcd->writeLn(0,0,line);    
}

void UserInterface::processMenu(Keys pressedKey){
     DPRINTLN_F("UserInterface::processMenu(%d)", pressedKey);
        switch (pressedKey)
        {
            case Keys::KEY_ENTER:
                menu->enter();
                break;
            case Keys::KEY_BACK:
                menu->reset();
                break;
            case Keys::KEY_UP:
                menu->moveNext();
                break;
            case Keys::KEY_DOWN:
                menu->movePrevious();
                break;
        };
    MenuItem  item = this->menu->getCurrentItem();
    this->showMenuScreen(item);
}

void UserInterface::setupMenuCallbacks(){

    this->menu->callerCallbackInstance = this;        
}

#pragma endregion