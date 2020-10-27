#include "headers\MenuActionBase.h"
#include "headers\DateTimeSetting.h"
#include "headers\Lcd.h"

DateTimeSetting::DateTimeSetting(Lcd* lcd): MenuActionBase(lcd)
{
}

void DateTimeSetting::begin()
{
    year = 2020;
    month = 1;
    day = 1;

    hour = 0;
    minute = 0;
}

void DateTimeSetting::update()
{
    if (millis() - lastBlinkUpdateTime > 500)
    {
        lastBlinkUpdateTime = millis();
        this->blink();
    }
}

void DateTimeSetting::initDateTime()
{
    lcd->writeLn(1, 0, "                \0");
    setDate = true;
    setTime = true;
    blinkOn = false;
    currentStatus = DATETIMESETTING_YEAR;
}

void DateTimeSetting::initTime()
{
    lcd->writeLn(1, 0, "                \0");
    setDate = false;
    setTime = true;
    blinkOn = false;
    currentStatus = DATETIMESETTING_HOUR;
}

void DateTimeSetting::keyEnter()
{
    switch (currentStatus)
    {
    case DATETIMESETTING_YEAR:
        currentStatus = DATETIMESETTING_MONTH;
        break;
    case DATETIMESETTING_MONTH:
        currentStatus = DATETIMESETTING_DAY;
        break;
    case DATETIMESETTING_DAY:
        currentStatus = DATETIMESETTING_HOUR;
        break;
    case DATETIMESETTING_HOUR:
        currentStatus = DATETIMESETTING_MINUTE;
        break;
    case DATETIMESETTING_MINUTE:
        //??currentStatus = DATETIMESETTING_MINUTE;
        break;
    default:
        break;
    }
}

void DateTimeSetting::keyUp()
{
    switch (currentStatus)
    {
    case DATETIMESETTING_YEAR:
        year++;
        break;
    case DATETIMESETTING_MONTH:
        if (++month > 12)
        {
            month = 1;
        }
        break;
    case DATETIMESETTING_DAY:
        if (++day > 31)
        {
            day = 1;
        }
        break;
    case DATETIMESETTING_HOUR:
        if (++hour > 23)
        {
            hour = 00;
        }
        break;
    case DATETIMESETTING_MINUTE:
        if (++minute > 59)
        {
            minute = 00;
        }
        break;
    }
}

void DateTimeSetting::keyDown()
{
    switch (currentStatus)
    {
    case DATETIMESETTING_YEAR:
        year--;
        break;
    case DATETIMESETTING_MONTH:
        if (--month < 1)
        {
            month = 12;
        }
        break;
    case DATETIMESETTING_DAY:
        if (--day <  1)
        {
            day = 31;
        }
        break;
    case DATETIMESETTING_HOUR:
        if (--hour < 00)
        {
            hour = 23;
        }
        break;
    case DATETIMESETTING_MINUTE:
        if (--minute < 00)
        {
            minute = 59;
        }
        break;
    }
}

void DateTimeSetting::keyBack(){

}

void DateTimeSetting::blink()
{
    blinkOn = !blinkOn;

    char line[16 + 1] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    switch (currentStatus)
    {
    case DATETIMESETTING_YEAR:
        if (blinkOn)
        {
            sprintf(line, "Year:%d\0", year);
        }
        else
        {
            sprintf(line, "Year:    \0");
        }
        break;
    case DATETIMESETTING_MONTH:
        if (blinkOn)
        {
            sprintf(line, "Month:%d \0", month);
        }
        else
        {
            sprintf(line, "Month:   \0");
        }
        break;
    case DATETIMESETTING_DAY:
        if (blinkOn)
        {
            sprintf(line, "Day:%d   \0", day);
        }
        else
        {
            sprintf(line, "Day:     \0");
        }
        break;
    case DATETIMESETTING_HOUR:
        if (blinkOn)
        {
            sprintf(line, "Hour:%d  \0", hour);
        }
        else
        {
            sprintf(line, "Hour:    \0");
        }
        break;
    case DATETIMESETTING_MINUTE:
        if (blinkOn)
        {
            sprintf(line, "Minute:%d\0", minute);
        }
        else
        {
            sprintf(line, "Minute:  \0");
        }
        break;
    default:
        break;
    };
    lcd->writeLn(1, 0, line);
}