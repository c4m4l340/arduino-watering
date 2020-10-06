#define DEBUG
#include "headers\SerialDebug.h"

#include "headers\WaterProgram.h"
#include "headers\LedRgb.h"

WaterProgram::WaterProgram(LedRgb* Led){
   this->Led = Led;
}

void WaterProgram::begin(){
    previousStatus = WATER_PROGRAM_STATUS_STANDBY;
    currentStatus =  WATER_PROGRAM_STATUS_STANDBY;
    Led->setColorGroup(blinkGroups[currentStatus]);
    //DPRINTLN_F("Led2: %p, %p, %p, %p",&Led, Led, &aLed, aLed);
}

void WaterProgram::update(){
    if(previousStatus != currentStatus){
        Led->setColorGroup(blinkGroups[currentStatus]);
        previousStatus = currentStatus;
    }
}

void WaterProgram::open(){
    currentStatus = WATER_PROGRAM_MANUAL_RUNNING;
}

void WaterProgram::close(){
    currentStatus = WATER_PROGRAM_STATUS_STANDBY;
}

void WaterProgram::abort(){
    currentStatus = WATER_PROGRAM_STATUS_STANDBY;
}