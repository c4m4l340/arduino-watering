#include "headers\SerialDebug.h"
#include "headers\Communications.h"
#include "headers\RS485.h"


Communications::Communications(int8_t receivePin, int8_t transmitPin, int8_t ctrlPin)
    : serialPort(receivePin,transmitPin)
{
    rxTxCtrlPin = ctrlPin;
}

void Communications::begin(long commSpeed)
{
    pinMode(rxTxCtrlPin, OUTPUT);
    digitalWrite(rxTxCtrlPin, SERIAL_RECEIVE);
    serialPort.begin(commSpeed);
    status = CommStatus::Standby;
}

void Communications::update()
{    
    unsigned long currentTime = millis();

    switch(status){
        case WaitResponse:
        if(currentTime - startCommunicationTime > COMM_TIMEOUT){
            setCommunicationTimeout();
        }
        else
        {
            if (serialPort.available())
            {
                //DPRINTLN("serialPort.available");
                byte buffer[responseBufferSize + 1];
                int c = serialPort.readBytesUntil('#', buffer, sizeof(responseBuffer) / sizeof(byte));
                //DPRINTLN_F("Received:%s",buffer);           
                memcpy(responseBuffer, buffer, c);
                setCommunicationReceived();
            }
        }
        break;
        case Standby:
        break;
    }
}


bool Communications::sendCommand(byte packet[], int size)
{    
    //DPRINTLN_F("Communications::sendCommand([%s], %d)", packet, size);
    
    digitalWrite(rxTxCtrlPin, SERIAL_TRANSMIT);
    int c = serialPort.write(packet, size);
    c += serialPort.write('#');
    serialPort.flush();

    digitalWrite(rxTxCtrlPin, SERIAL_RECEIVE);
    status = CommStatus::WaitResponse;
    startCommunicationTime = millis();

    if(onCommunicationStarted != NULL){
        onCommunicationStarted(packet, size);
    }
}


void Communications::setCommunicationTimeout()
{
    if(onCommunicationTimeout != NULL){
        onCommunicationTimeout();
    }
    status = CommStatus::Standby;
}

void Communications::setCommunicationReceived()
{
    if(onCommunicationReceived != NULL){
        onCommunicationReceived(responseBuffer, responseBufferSize);
    }
    status = CommStatus::Standby;
}