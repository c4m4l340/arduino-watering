#ifndef _COMMUNICATIONS_H_
#define _COMMUNICATIONS_H_

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "SerialDebug.h"

#define COMM_TIMEOUT 10000

enum CommStatus: byte
{
    Standby = 0x00,
    WaitResponse = 0x01    
};

using CommunicationStartedEventDelegate = void (*)(byte buffer[], int size);
using CommunicationReceivedEventDelegate = void (*)(byte buffer[], int size);
using CommunicationTimeoutEventDelegate = void (*)();

class Communications{

    private:
        SoftwareSerial serialPort;
        
        int8_t rxTxCtrlPin;
        long commSpeed;

        unsigned long previousTime;
        unsigned long startCommunicationTime;
        static const size_t responseBufferSize = 4;
        byte responseBuffer[responseBufferSize];
        
        void setCommunicationTimeout();
        void setCommunicationReceived();
        
        
    public:
        CommStatus status = CommStatus::Standby;
        CommunicationStartedEventDelegate onCommunicationStarted = NULL;
        CommunicationReceivedEventDelegate onCommunicationReceived = NULL;
        CommunicationTimeoutEventDelegate onCommunicationTimeout = NULL;
        
        Communications(int8_t receivePin, int8_t transmitPin, int8_t ctrlPin);
        void begin(long commSpeed);
        void update();   

        bool sendCommand(byte packet[], int size);  
};

#endif