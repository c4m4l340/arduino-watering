#ifndef __MENU_ACTION_BASE__
#define __MENU_ACTION_BASE__

#include "Lcd.h"

using ActioncCompleteCallback = void (*)(void* caller_ptr, byte tag);
using ActionCanceledCallback = void (*)(void* caller_ptr, byte tag);

class MenuActionBase{

    protected:
        Lcd* lcd;

    public:
        byte tag;
        
        ActioncCompleteCallback onActioncComplete= NULL;
        ActionCanceledCallback onActionCanceled = NULL;
        void* callerCallbackInstance = NULL;

        MenuActionBase(){};

        virtual void begin(Lcd* lcd) = 0;
        virtual void update() = 0;
        virtual void reset (byte tag) = 0;
        virtual void keyUp() = 0;
        virtual void keyDown() = 0;
        virtual void keyEnter() = 0;
        virtual void keyBack() = 0;
};

#endif