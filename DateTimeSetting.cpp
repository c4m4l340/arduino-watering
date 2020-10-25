#include "headers\DateTimeSetting.h"
#include "headers\Lcd.h"
DateTimeSetting::DateTimeSetting(Lcd* lcd){
    this->lcd = lcd;
}

void DateTimeSetting::begin(){
    year = 2020;
    month=1;
    day=1;

    hour=0;
    minute=0;
}

void DateTimeSetting::update(){
    if(millis() - lastBlinkUpdateTime > 500 ){
        lastBlinkUpdateTime = millis();
        this->blink();
    }
    
}

void DateTimeSetting::initDateTime(){
    lcd->writeLn(1,0,"                \0");
    setDate = true;
    setTime = true;
    blinkOn = false;
    currentStatus = DATETIMESETTING_YEAR;
}

void DateTimeSetting::initTime(){
     lcd->writeLn(1,0,"                \0");
    setDate = false;
    setTime = true;
    blinkOn = false;
    currentStatus = DATETIMESETTING_HOUR;
}

void DateTimeSetting::next(){

}

void DateTimeSetting::up(){

}

void DateTimeSetting::down(){

}

void DateTimeSetting::blink(){
    blinkOn = !blinkOn;
    
    char line[16+1] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    switch (currentStatus)
    {
    case DATETIMESETTING_YEAR:        
        if(blinkOn){
            sprintf(line,"%d\0",year);
        }
        else{
             sprintf(line,"    \0");
        }
        break;
    case DATETIMESETTING_MONTH:        
        if(blinkOn){
            sprintf(line,"%d\0",month);
        }
        else{
             sprintf(line,"  \0");
        }
        break;
    case DATETIMESETTING_DAY:        
        if(blinkOn){
            sprintf(line,"%d\0",day);
        }
        else{
             sprintf(line,"  \0");
        }
        break;
    case DATETIMESETTING_HOUR:        
        if(blinkOn){
            sprintf(line,"%d\0",hour);
        }
        else{
             sprintf(line,"  \0");
        }
        break;
    case DATETIMESETTING_MINUTE:        
        if(blinkOn){
            sprintf(line,"%d\0",minute);
        }
        else{
             sprintf(line,"  \0");
        }
        break;
    default:
        break;
    };
    lcd->writeLn(1,0,line);
}