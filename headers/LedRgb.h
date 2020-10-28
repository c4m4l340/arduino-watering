#ifndef _LEDRGB_H_
#define _LEDRGB_H_

#include "Arduino.h"

struct LedColor {
    byte r;
    byte g;
    byte b;
    long time;
};

typedef LedColor BlinkGroup[2];

class LedRgb {
    private:
        /* OutputPins*/
        byte pinRed;
        byte pinGreen;
        byte pinBlue;

        /**/
        BlinkGroup colorGroup = {{255,255,255,75},{0,0,0,75}};

        /*blink led*/
        unsigned long previousBlinkTime;   
        byte colorIdx;

        void setColor(LedColor color);

    public: 
        LedRgb(byte redPin, byte greenPin, byte bluePin);
        //LedRgb();
        
        void begin();
        void update();

         void setColorGroup(BlinkGroup group);
};

#endif