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
WaterProgram WProgram(&Led, &Comms);

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
     

    
    Keypad.begin();
    Keypad.onKeyDown = onKeyDown;
    Keypad.onKeyUp = onKeyUp;

    Clock.begin();
    Clock.onGetTime = onGetTime;

    Lcd.begin();
    Lcd.onSleep = onLcdSleep;
    Lcd.onWakeup = onLcdWakeup;

    Led.begin(); 
    Comms.begin(RS485_SPEED);
    WProgram.begin();

}

void loop(){
    
    Comms.update();
    Keypad.update();
    Clock.update();
    Lcd.update();

    Led.update();
    WProgram.update();
}

void onKeyDown(int key){
    DPRINTLN_F("onKeyDown(%d)", key);
}

void onKeyUp(int key){
    DPRINTLN_F("onKeyUp(%d)", key);

    Lcd.wakeup();
    switch(key){
        case Keys::KEY_UP: WProgram.open(); break;
        case Keys::KEY_DOWN: WProgram.close();break;
        case Keys::KEY_BACK:WProgram.abort();break;
        case Keys::KEY_ENTER:WProgram.check(0,0,0);
    }
}

void onGetTime(DateTime datetime){
     DPRINTLN_F("onGetTime: %02d-%02d-%02d %02d:%02d:%02d", 
     datetime.year(), datetime.month(), datetime.day(),
     datetime.hour(), datetime.minute(), datetime.second());

    // WProgram.check(datetime.hour(), datetime.minute(), datetime.second());
}

void onLcdSleep(){
        DPRINTLN("onLcdSleep");
}

void onLcdWakeup(){
    DPRINTLN("onLcdWakeup");
}