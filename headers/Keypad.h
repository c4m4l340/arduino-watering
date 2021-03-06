#define DEBUG

#ifndef _KEYPAD_H_
#define _KEYPAD_H_

#include "SerialDebug.h"

    enum Keys{
            KEY_UP = 4,
            KEY_DOWN = 3,
            KEY_BACK = 2,
            KEY_ENTER = 1,
            KEY_NO_KEY = 0,
    };

using KeyDownCallback = void (*)(int key, void* caller_ptr);
using KeyUpCallback = void (*)(int key, void* caller_ptr);

    class Keypad{
        
        private:
            int inputPin;
            int const DEBOUCE_KEY_VALUE = 2;
            int const  KEY_NO_KEY_VALUE = 650;
            int const  KEY_BACK_VALUE = 470;
            int const  KEY_DOWN_VALUE = 325;
            int const  KEY_UP_VALUE = 160;
            int const  KEY_ENTER_VALUE = 50;
            
            Keys readKeys[3];
            byte keyReadingCount;
            int lastPressedKey;

            unsigned long previousDebounceTime = 0;
            
        public:
            KeyDownCallback onKeyDown = NULL;
            KeyUpCallback onKeyUp = NULL;
            void* callerCallbackInstance = NULL;
            
            Keypad(byte pin);
            void begin();
            void update();
    };

#endif