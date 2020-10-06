#define DEBUG
#include "headers\SerialDebug.h"

#include "headers\LedRgb.h"


LedRgb::LedRgb(int redPin, int greenPin, int bluePin)
{
    pinRed = redPin;
    pinGreen = greenPin;
    pinBlue = bluePin;
}
//LedRgb::LedRgb(){}


void LedRgb::begin()
{
    pinMode(pinRed, OUTPUT);
    pinMode(pinGreen, OUTPUT);
    pinMode(pinBlue, OUTPUT);

    previousBlinkTime = 0;
    colorIdx = 0;
};

void LedRgb::update()
{
    unsigned long currentTime = millis();

    if(((currentTime - previousBlinkTime) > colorGroup[colorIdx].time)){
         previousBlinkTime = currentTime;
         colorIdx = colorIdx== 0 ? 1 : 0;
         setColor(colorGroup[colorIdx]);
    }
}


 void LedRgb::setColorGroup(BlinkGroup group){
     memcpy(colorGroup, group,sizeof(BlinkGroup));
    //  colorGroup[0].r = group[0].r;
    //  colorGroup[0].g = group[0].g;
    //  colorGroup[0].b = group[0].b;
    //  colorGroup[0].time = group[0].time;

    //  colorGroup[1].r = group[1].r;
    //  colorGroup[1].g = group[1].g;
    //  colorGroup[1].b = group[1].b;
    //  colorGroup[1].time = group[1].time;

     colorIdx=0;
     setColor(colorGroup[colorIdx]);
     previousBlinkTime = 0;

 }


void LedRgb::setColor(LedColor color)
{
    analogWrite(pinRed, color.r );
    analogWrite(pinGreen, color.g );
    analogWrite(pinBlue, color.b );
}


