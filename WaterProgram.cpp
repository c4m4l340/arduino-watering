#include "headers\SerialDebug.h"

#include "headers\WaterProgram.h"
#include "headers\LedRgb.h"
#include "headers\Communications.h"

WaterProgram::WaterProgram(LedRgb* led, Communications* comms){
   this->led = led;
   this->comms = comms;
}

#pragma region Task setup and run
void WaterProgram::begin(){
    previousStatus = WATER_PROGRAM_STATUS_STANDBY;
    currentStatus =  WATER_PROGRAM_STATUS_STANDBY;

    led->setColorGroup(blinkGroups[currentStatus]);
    comms->onCommunicationStarted = onCommunicationStarted;
    comms->onCommunicationReceived = onCommunicationReceived;
    comms->onCommunicationTimeout = onCommunicationTimeout;
    comms->callbackCallerInstance = this;    
    
    //DPRINTLN_F("Led2: %p, %p, %p, %p",&Led, Led, &aLed, aLed);
}

void WaterProgram::update(){
    if(previousStatus != currentStatus){
        led->setColorGroup(blinkGroups[currentStatus]);
        previousStatus = currentStatus;
    }
}
#pragma endregion

#pragma region Actions

void WaterProgram::open(){
    DPRINTLN("open");
    if(comms->status == CommStatus::Standby){
        byte buffer[] = {'O','P','E','N'};
        comms->sendCommand(buffer,4);
        currentStatus = WATER_PROGRAM_MANUAL_RUNNING;
    } 
    
}

void WaterProgram::close(){
    DPRINTLN("close");
    if(comms->status == CommStatus::Standby){
        byte buffer[] = {'C','L','O','S','E'};
        comms->sendCommand(buffer,5);
        currentStatus = WATER_PROGRAM_STATUS_STANDBY;
    } 
}

void WaterProgram::abort(){
    DPRINTLN("abort");
     if(comms->status == CommStatus::Standby){
        byte buffer[] = {'A','B','O','R','T'};
        comms->sendCommand(buffer,5);
        currentStatus = WATER_PROGRAM_STATUS_STANDBY;
    } 
}

void WaterProgram::pushTime(byte hours, byte minutes, byte seconds){
     DPRINTLN_F("WaterProgram::pushTime(%d,%d,%d)", hours, minutes, seconds);

    if( (currentStatus != WATER_PROGRAM_SCHEDULED_RUNNING) && 
        (currentStatus != WATER_PROGRAM_MANUAL_RUNNING) &&
        isTimeToRun(hours, minutes, seconds)){
        runScheduled();
    }
}
#pragma endregion

#pragma region Private

bool WaterProgram::isTimeToRun(byte hours, byte minutes, byte seconds){
    DPRINTLN("isTimeToRun: return FALSE!!!!!!!");
    return false;
}

void WaterProgram::runScheduled(){
    //DPRINTLN("runScheduled");
    if(comms->status == CommStatus::Standby){
        byte buffer[] = {'R','U','N'};
        comms->sendCommand(buffer,3);
        currentStatus = WATER_PROGRAM_SCHEDULED_RUNNING;
    }    
}

#pragma endregion

#pragma region Callbacks
void WaterProgram::onCommunicationStarted(byte packet[], int size, void* this_ptr){
    byte tmp[size+1];
    memcpy(tmp, packet, size);
    tmp[size]=0;
    DPRINTLN_F("onCommunicationStarted: [%s], %d",tmp,size);
}

void WaterProgram::onCommunicationTimeout(void* this_ptr){
    DPRINTLN("onCommunicationTimeout");

    WaterProgram* me = static_cast<WaterProgram*>(this_ptr);
    me->currentStatus = WATER_PROGRAM_STATUS_STANDBY;
    
}

void WaterProgram::onCommunicationReceived(byte buffer[], int size, void* this_ptr){
     byte tmp[size+1];
    memcpy(tmp, buffer, size);
    tmp[size]=0;
    DPRINTLN_F("onCommunicationReceived: [%s], %d",tmp,size);
}
#pragma endregion
