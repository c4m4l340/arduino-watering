#define DEBUG

#include <Arduino.h>
#include "headers\UserInterface.h"
#include "headers\Lcd.h"
#include "headers\Keypad.h"
#include "headers\MenuActionSetDateTime.h"
#include <WString.h>

UserInterface::UserInterface(Lcd* lcd, Keypad* keypad, Menu* menu, WaterProgram* wprogram){
    this->lcd = lcd;
    this->keypad = keypad;
    this->menu = menu;
    this->wprogram = wprogram;
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

    setMenuCallbacks();
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
        case USER_INTERFACE_EXEC_ACTION:{
            this->menuAction->update();
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
                    me->menu->reset();
                    me->processMenu(Keys::KEY_NO_KEY);
                }
            break;
        } 
        case USER_INTERFACE_STATUS_USING_MENU:{
            me->processMenu(key);
            break;
        }
        case USER_INTERFACE_EXEC_ACTION:{
            me->processAction(key);
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

void UserInterface::onActionComplete(void* data, void* caller_ptr){
    UserInterface* me = static_cast<UserInterface*>(caller_ptr);
    me->currentStatus = USER_INTERFACE_STATUS_STANDBY;
    DPRINTLN("UserInterface::onActionComplete");
}

void UserInterface::onActionCanceled (void* caller_ptr){
    UserInterface* me = static_cast<UserInterface*>(caller_ptr);
    me->currentStatus = USER_INTERFACE_STATUS_STANDBY;
    DPRINTLN("UserInterface::onActionCanceled");
}

void UserInterface::execMenuOpen(void* caller_ptr){
    UserInterface* me = static_cast<UserInterface*>(caller_ptr);
    me->wprogram->open();
}

void UserInterface::execMenuClose(void* caller_ptr){
    UserInterface* me = static_cast<UserInterface*>(caller_ptr);
    me->wprogram->abort();
}

void UserInterface::execSetDateTime(void* caller_ptr){
    UserInterface* me = static_cast<UserInterface*>(caller_ptr);
     me->currentStatus = USER_INTERFACE_EXEC_ACTION;
     MenuActionSetDateTime* action = new MenuActionSetDateTime(me->lcd); 
     action->begin();
     action->initDateTime();
     action->callerCallbackInstance = me;
     action->onActioncComplete = onActionComplete;
     action->onActionCanceled = onActionCanceled;
     me->menuAction = action;
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

void UserInterface::showMenuScreen(MenuItem* item){
    //DPRINTLN_F("UserInterface::showMenuScreen{%d,%s}:currentStatus=%d", item.title.c_str(), this->currentStatus);
    
    char line[16+1] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    lcd->writeLn(1,0,"                \0");
    lcd->writeLn(0,0,"                \0");

    snprintf(line,16+1,"%s%c",item->title.c_str(), (item->hasChildren()?0x7E:0x00));
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
                if(menu->isCurrentItemRoot()){
                    this->currentStatus = USER_INTERFACE_STATUS_STANDBY;
                    return;
                }else{
                    menu->reset();
                }
                break;
            case Keys::KEY_UP:
                menu->moveNext();
                break;
            case Keys::KEY_DOWN:
                menu->movePrevious();
                break;
        };
    MenuItem* item = this->menu->getCurrentItem();
    this->showMenuScreen(item);
}

void UserInterface::processAction(Keys pressedKey){
        DPRINTLN_F("UserInterface::processSettingDateTime(%d)", pressedKey);
        switch (pressedKey)
        {
            case Keys::KEY_ENTER:
                menuAction->keyEnter();
                break;
            case Keys::KEY_BACK:
                //cancel
                menuAction->keyBack();
                break;
            case Keys::KEY_UP:
                menuAction->keyUp();
                break;
            case Keys::KEY_DOWN:
                menuAction->keyDown();                
        };
}

void UserInterface::setMenuCallbacks(){
    //get root item
    menu->callerCallbackInstance = this;
    menu->reset();
    MenuItem* root = menu->getCurrentItem();
    (root+0)->action = execMenuOpen;
    (root+1)->action = execMenuClose;
    (root+2)->action = execSetDateTime;
}
#pragma endregion