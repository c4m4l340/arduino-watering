#define DEBUG
#include "headers\SerialDebug.h"

#include "headers\MenuActionBase.h"
#include "headers\MenuActionSetDateTime.h"
#include "headers\Lcd.h"

MenuActionSetDateTime::MenuActionSetDateTime() : MenuActionBase()
{
}

void MenuActionSetDateTime::begin(Lcd *lcd)
{
    this->lcd = lcd;
}

void MenuActionSetDateTime::update()
{
    if (millis() - lastBlinkUpdateTime > 500)
    {
        lastBlinkUpdateTime = millis();
        this->blink();
    }
}

void MenuActionSetDateTime::initSetDateTime()
{
    setDateTime = true;
    setScheduler = false;
}

void MenuActionSetDateTime::initSetScheduler()
{
    setDateTime = false;
    setScheduler = true;
}

void MenuActionSetDateTime::reset(byte tag)
{

    this->tag = tag;

    this->datetime.year = 2020;
    this->datetime.month = 1;
    this->datetime.day = 1;
    this->datetime.hour = 0;
    this->datetime.minute = 0;
    this->datetime.duration = 5;    

    lcd->writeLn(1, 0, "                \0");
    blinkOn = false;

    if (setDateTime)
    {
        currentStatus = DATETIMESETTING_YEAR;
    }
    else
    {
        currentStatus = DATETIMESETTING_HOUR;
    }
}

void MenuActionSetDateTime::keyEnter()
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
        if (setScheduler)
        {
            currentStatus = DATETIMESETTING_DURATION;
            break;
        } //else fall down
    case DATETIMESETTING_DURATION:
        currentStatus = DATETIMESETTING_YEAR;
        if (onActioncComplete != NULL)
        {
            onActioncComplete(callerCallbackInstance, tag);
        }
        break;
    default:
        break;
    }
}

void MenuActionSetDateTime::keyUp()
{
    switch (currentStatus)
    {
    case DATETIMESETTING_YEAR:
        (this->datetime.year)++;
        break;
    case DATETIMESETTING_MONTH:
        if (++(this->datetime.month) > 12)
        {
            this->datetime.month = 1;
        }
        break;
    case DATETIMESETTING_DAY:
        if (++(this->datetime.day) > 31)
        {
            this->datetime.day = 1;
        }
        break;
    case DATETIMESETTING_HOUR:
        if (++(this->datetime.hour) > 23)
        {
            this->datetime.hour = 00;
        }
        break;
    case DATETIMESETTING_MINUTE:
        if (++(this->datetime.minute) > 59)
        {
            this->datetime.minute = 00;
        }
        break;
    case DATETIMESETTING_DURATION:
        if (++(this->datetime.duration) > 60)
        {
            this->datetime.duration = 00;
        }
        break;
    }
}

void MenuActionSetDateTime::keyDown()
{
    switch (currentStatus)
    {
    case DATETIMESETTING_YEAR:
        this->datetime.year--;
        break;
    case DATETIMESETTING_MONTH:
        if (--(this->datetime.month) < 1)
        {
            this->datetime.month = 12;
        }
        break;
    case DATETIMESETTING_DAY:
        if (--(this->datetime.day) < 1)
        {
            this->datetime.day = 31;
        }
        break;
    case DATETIMESETTING_HOUR:
        if (this->datetime.hour == 00)
        {
            this->datetime.hour = 24;
        }
        this->datetime.hour--;
        break;
    case DATETIMESETTING_MINUTE:
        if (this->datetime.minute == 00)
        {
            this->datetime.minute = 60;
        }
        this->datetime.minute--;
        break;
    case DATETIMESETTING_DURATION:
        if (this->datetime.duration == 00)
        {
            this->datetime.duration = 61;
        }
        this->datetime.duration--;
    }
}

void MenuActionSetDateTime::keyBack()
{
    if (onActionCanceled != NULL)
    {
        onActionCanceled(callerCallbackInstance, tag);
    }
}

void MenuActionSetDateTime::blink()
{
    blinkOn = !blinkOn;

    char line[16 + 1] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    switch (currentStatus)
    {
    case DATETIMESETTING_YEAR:
        if (blinkOn)
        {
            sprintf(line, "Year:%d\0", this->datetime.year);
        }
        else
        {
            sprintf(line, "Year:    \0");
        }
        break;
    case DATETIMESETTING_MONTH:
        if (blinkOn)
        {
            sprintf(line, "Month:%d \0", this->datetime.month);
        }
        else
        {
            sprintf(line, "Month:   \0");
        }
        break;
    case DATETIMESETTING_DAY:
        if (blinkOn)
        {
            sprintf(line, "Day:%d  \0", this->datetime.day);
        }
        else
        {
            sprintf(line, "Day:    \0");
        }
        break;
    case DATETIMESETTING_HOUR:
        if (blinkOn)
        {
            sprintf(line, "Hour:%d \0", this->datetime.hour);
        }
        else
        {
            sprintf(line, "Hour:   \0");
        }
        break;
    case DATETIMESETTING_MINUTE:
        if (blinkOn)
        {
            sprintf(line, "Minute:%d\0", this->datetime.minute);
        }
        else
        {
            sprintf(line, "Minute:  \0");
        }
        break;
    case DATETIMESETTING_DURATION:
        if (blinkOn)
        {
            sprintf(line, "Duration:%d\0", this->datetime.duration);
        }
        else
        {
            sprintf(line, "Duration:  \0");
        }
        break;

    default:
        break;
    };
    lcd->writeLn(1, 0, line);
}

void MenuActionSetDateTime::getSettedDateTime(MenuDateTime *dt)
{
    memcpy(dt, &datetime,sizeof(MenuDateTime));
}

void MenuActionSetDateTime::getSettedScheduler(byte& hour, byte& minute, byte& duration){
     hour = this->datetime.hour;
     minute = this->datetime.minute;
     duration = this->datetime.duration;
}