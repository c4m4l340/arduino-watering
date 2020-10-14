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
#include "headers\UserInterface.h"
#include "headers\MenuItemsSetup.h"

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


Clock Clock; //parameterless constructor

Communications Comms(RS485_RX, RS485_TX, RS485_TXRX_SELECTOR);
LedRgb Led(PIN_RGB_RED, PIN_RGB_GREEN, PIN_RGB_BLUE);
WaterProgram WProgram(&Led, &Comms);

Menu Menu(itemsRoot, sizeof(itemsRoot) / sizeof(MenuItem));
Keypad Keypad(KEYPAD_PIN);
Lcd Lcd(LCD_I2C_ADDRESS, 16, 2);
UserInterface UserInterface(&Lcd, &Keypad, &WProgram, &Menu);

//callback declarations
void onGetTime(DateTime datetime, void* caller_ptr);


//ProgramStatus = Standby, ManualRunning, ScheduledRunning
//CommStatus = Standby, WaitResponse
//UserInteractionStatus = Off, Standby,  Menu

void setup(){
    DINITSERIAL(9600);

    Clock.begin();
    Clock.onGetTime = onGetTime;
    Clock.callerCallbackInstance = nullptr;

    Led.begin(); 
    Comms.begin(RS485_SPEED);
    WProgram.begin();

    Menu.begin();
    Lcd.begin();
    Keypad.begin();    
    UserInterface.begin();
}

void loop(){
    
    Comms.update();
    Keypad.update();
    Clock.update();
    Lcd.update();

    Menu.update();
    Led.update();
    WProgram.update();
    UserInterface.update();
}


void onGetTime(DateTime datetime, void* caller_ptr){
     DPRINTLN_F("onGetTime: %02d-%02d-%02d %02d:%02d:%02d", 
     datetime.year(), datetime.month(), datetime.day(),
     datetime.hour(), datetime.minute(), datetime.second());

    //WProgram.pushTime(datetime.hour(), datetime.minute(), datetime.second());
    UserInterface.pushTime(datetime.hour(), datetime.minute(), datetime.second());
}

