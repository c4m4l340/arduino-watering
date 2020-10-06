#define DEBUG

//https://github.com/mrkaleArduinoLib/LiquidCrystal_I2C#noBacklight

#include "headers\SerialDebug.h"
#include "headers\LedRgb.h"
#include "headers\RS485.h"
#include "headers\Communications.h"
#include "headers\Keypad.h"
#include "headers\Clock.h"
#include "headers\Lcd.h"
#include "headers\WaterProgram.h"

/* OutputPins*/
#define PIN_RGB_RED 5
#define PIN_RGB_GREEN 6
#define PIN_RGB_BLUE 9

/*Input pins*/
#define KEYPAD_PIN A0


//Not all pins on the Leonardo and Micro support change interrupts, 
//so only the following can be used for RX: 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).
#define RS485_TX 11
#define RS485_RX 10
#define RS485_TXRX_SELECTOR 12

#define LCD_I2C_ADDRESS 0x27

Communications Comms(RS485_RX, RS485_TX, RS485_TXRX_SELECTOR);
Keypad Keypad(KEYPAD_PIN);
Clock Clock; //parameterless constructor
Lcd Lcd(LCD_I2C_ADDRESS, 16, 2);

LedRgb Led(PIN_RGB_RED, PIN_RGB_GREEN, PIN_RGB_BLUE);
WaterProgram WProgram(&Led);

//callback declarations
void onCommunicationStarted(byte packet[], int size);
void onCommunicationReceived(byte buffer[], int size);
void onCommunicationTimeout();
void onKeyDown(int key);
void onKeyUp(int key);
void onGetTime(DateTime datetime);


//ProgramStatus = Standby, ManualRunning, ScheduledRunning
//CommStatus = Standby, WaitResponse
//UserInteractionStatus = Off, Standby,  Menu

void setup(){
    DINITSERIAL(9600);
       
    Comms.begin(RS485_SPEED);
    Comms.onCommunicationStarted = onCommunicationStarted;
    Comms.onCommunicationReceived = onCommunicationReceived;
    Comms.onCommunicationTimeout = onCommunicationTimeout;
    
    Keypad.begin();
    Keypad.onKeyDown = onKeyDown;
    Keypad.onKeyUp = onKeyUp;

    Clock.begin();
    Clock.onGetTime = onGetTime;

    Lcd.begin();
    Lcd.onSleep = onLcdSleep;
    Lcd.onWakeup = onLcdWakeup;

    Led.begin(); 
    WProgram.begin();

}

void loop(){
    
    Comms.update();
    Keypad.update();
    Clock.update();
    Lcd.update();

    Led.update();
    WProgram.update();

    if(Comms.status == CommStatus::Standby){
        if(millis() % 5000 == 0){

             byte buffer[] = {'H','E','L','L','O'};
             Comms.sendCommand(buffer,5);
        }
    }
}

void onCommunicationStarted(byte packet[], int size){
    byte tmp[size+1];
    memcpy(tmp, packet, size);
    tmp[size]=0;
    DPRINTLN_F("onCommunicationStarted: [%s], %d",tmp,size);
}

void onCommunicationTimeout(){
    DPRINTLN("onCommunicationTimeout");
}

void onCommunicationReceived(byte buffer[], int size){
     byte tmp[size+1];
    memcpy(tmp, buffer, size);
    tmp[size]=0;
    DPRINTLN_F("onCommunicationReceived: [%s], %d",tmp,size);
}

void onKeyDown(int key){
    DPRINTLN_F("onKeyDown(%d)", key);
}

void onKeyUp(int key){
    DPRINTLN_F("onKeyUp(%d)", key);

    Lcd.wakeup();
}

void onGetTime(DateTime datetime){
     DPRINTLN_F("onGetTime: %02d-%02d-%02d %02d:%02d:%02d", 
     datetime.year(), datetime.month(), datetime.day(),
     datetime.hour(), datetime.minute(), datetime.second());
}

void onLcdSleep(){
        DPRINTLN("onLcdSleep");
}

void onLcdWakeup(){
    DPRINTLN("onLcdWakeup");
}