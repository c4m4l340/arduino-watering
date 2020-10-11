#include "headers\SerialDebug.h"
#include "headers\Lcd.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WString.h>


Lcd::Lcd(uint8_t address, uint8_t cols, uint8_t rows) : lcd(address, cols, rows, LCD_5x8DOTS)
{
}

#pragma region Task setup and run
void Lcd::begin()
{
    Wire.begin();
    lcd.begin();
	lcd.clear();
	this->off();
}
void Lcd::update()
{
    unsigned long currentTime = millis();

    //DPRINTLN_F("Lcd::update::isBacklightOn:%d::elapsed:%d",isBacklightOn,(currentTime - previousUpdateTime));
    if(isAwake)
    {
        if(currentTime - timerLightOn > LCD_LIGHT_TIMEOUT){
            this->off();
        }
    }

}
#pragma endregion

#pragma region Actions
void Lcd::wakeup(){
   this->on();
   if(onWakeup != NULL){
       onWakeup(callerCallbackInstance);
   }
}

void Lcd::writeLn(byte row, byte col, char line[]){
        DPRINTLN_F("Lcd::writeLn(%d, %d, %s)", row,col ,line);
        lcd.setCursor(col, row);
        lcd.print(line);
}

void Lcd::clear(){
    lcd.clear();
}


#pragma endregion

#pragma region Private
void Lcd::off(){
     lcd.noBacklight(); 
     isAwake=false;
     if(onSleep != NULL){
         onSleep(callerCallbackInstance);
     }
}

void Lcd::on(){
 lcd.backlight(); 
 isAwake=true;
}

#pragma endregion

void Lcd::resetLightOnTimeout(){
    this->timerLightOn = millis();
}

// void Lcd::showTimeAndDate(int col, int row)
// {
//     unsigned long currentTime = millis();

//     if ((currentTime - lastIdleUpdateTime) > 1000)
//     {
//         lastIdleUpdateTime = currentTime;
//         bool h12;
//         bool PM;
//         bool Century;
//         sprintf(currentDateBuffer, "%02d.%02d.20%02d", ds3231.getDate(), ds3231.getMonth(Century), ds3231.getYear());
//         sprintf(currentTimeBuffer, "%02d:%02d:%02d", ds3231.getHour(h12, PM), ds3231.getMinute(), ds3231.getSecond());

//         lcd.setCursor(col, row);
//         lcd.print(currentDateBuffer);
//         lcd.setCursor(col, row + 1);
//         lcd.print(currentTimeBuffer);
//     }
// }

