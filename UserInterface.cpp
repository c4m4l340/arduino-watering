#define DEBUG

#include <Arduino.h>
#include "headers\UserInterface.h"
#include "headers\Lcd.h"
#include "headers\Keypad.h"

UserInterface::UserInterface(Lcd* lcd, Keypad* keypad, WaterProgram* waterProgram){
    this->lcd = lcd;
    this->keypad = keypad;
    this->waterProg = waterProgram;
}

void UserInterface::begin(){
    keypad->onKeyUp = onKeyUp;
    keypad->onKeyDown = onKeyDown;
    keypad->callerCallbackInstance = this;

    lcd->onSleep = onLcdSleep;
    lcd->onWakeup = onLcdWakeup;
    lcd->callerCallbackInstance = this;
}

void UserInterface::update(){

}

/** callbacks **/
void UserInterface::onKeyUp(int key, void* caller_ptr){
    DPRINTLN_F("onKeyUp(%d)", key);

    UserInterface* me = static_cast<UserInterface*>(caller_ptr);
    me->lcd->wakeup();
    switch(key){
        case Keys::KEY_UP: me->waterProg->open(); break;
        case Keys::KEY_DOWN: me->waterProg->close();break;
        case Keys::KEY_BACK: me->waterProg->abort();break;
        case Keys::KEY_ENTER: me->waterProg->check(0,0,0);
    }
}

void UserInterface::onKeyDown(int key, void* caller_ptr){
     DPRINTLN_F("onKeyDown(%d)", key);
}

void UserInterface::onLcdSleep(void* caller_ptr){
        DPRINTLN("onLcdSleep");
}

void UserInterface::onLcdWakeup(void* caller_ptr){
    DPRINTLN("onLcdWakeup");
}