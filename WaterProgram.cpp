#define DEBUG
#include "headers\SerialDebug.h"

#include "headers\WaterProgram.h"
#include "headers\LedRgb.h"
#include "headers\Communications.h"

WaterProgram::WaterProgram(LedRgb* Led, Communications* Comms){
   this->Led = Led;
   this->Comms = Comms;
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
    DPRINTLN("open");
    if(Comms->status == CommStatus::Standby){
        byte buffer[] = {'O','P','E','N'};
        Comms->sendCommand(buffer,4);
        currentStatus = WATER_PROGRAM_MANUAL_RUNNING;
    } 
    
}

void WaterProgram::close(){
    DPRINTLN("close");
    if(Comms->status == CommStatus::Standby){
        byte buffer[] = {'C','L','O','S','E'};
        Comms->sendCommand(buffer,5);
        currentStatus = WATER_PROGRAM_STATUS_STANDBY;
    } 
}

void WaterProgram::abort(){
    DPRINTLN("abort");
     if(Comms->status == CommStatus::Standby){
        byte buffer[] = {'A','B','O','R','T'};
        Comms->sendCommand(buffer,5);
        currentStatus = WATER_PROGRAM_STATUS_STANDBY;
    } 
}

void WaterProgram::check(int hours, int minutes, int seconds){
    if( (currentStatus != WATER_PROGRAM_SCHEDULED_RUNNING) && 
        (currentStatus != WATER_PROGRAM_MANUAL_RUNNING) &&
        isTimeToRun(hours, minutes, seconds)){
        run();
    }
}

/******************* private *********************************/

bool WaterProgram::isTimeToRun(int hours, int minutes, int seconds){
    DPRINTLN("isTimeToRun");
    return true;
}

void WaterProgram::run(){
    DPRINTLN("run");
    if(Comms->status == CommStatus::Standby){
        byte buffer[] = {'R','U','N'};
        Comms->sendCommand(buffer,3);
        currentStatus = WATER_PROGRAM_SCHEDULED_RUNNING;
    }    
}