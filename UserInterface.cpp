#define DEBUG

#include <Arduino.h>
#include "headers\UserInterface.h"
#include "headers\Lcd.h"
#include "headers\Keypad.h"
#include "headers\MenuActionSetDateTime.h"
#include <WString.h>

UserInterface::UserInterface(Lcd *lcd, Keypad *keypad, Menu *menu, WaterProgram *wprogram)
{
    this->lcd = lcd;
    this->keypad = keypad;
    this->menu = menu;
    this->wprogram = wprogram;
}

#pragma region Task setup and run
void UserInterface::begin()
{
    this->currentStatus = USER_INTERFACE_STATUS_OFF;

    keypad->onKeyUp = onKeyUp;
    keypad->onKeyDown = onKeyDown;
    keypad->callerCallbackInstance = this;

    lcd->onSleep = onLcdSleep;
    lcd->onWakeup = onLcdWakeup;
    lcd->callerCallbackInstance = this;

    menuActionSetDateTime->begin(this->lcd);
    menuActionSetDateTime->callerCallbackInstance = this;
    menuActionSetDateTime->onActioncComplete = onActionComplete;
    menuActionSetDateTime->onActionCanceled = onActionCanceled;

    //get root item
    menu->callerCallbackInstance = this;
    menu->reset();
    MenuItem *root = menu->getCurrentItem();
    (root + 0)->callback = execMenuOpen;
    (root + 1)->callback = execMenuClose;
    (root + 2)->callback = execMenuActionSetDateTime;
    //(root + 3) -> Programs
    (((root + 3)->childs)+0)->callback = execMenuActionSetScheduler1;
    (((root + 3)->childs)+1)->callback = execMenuActionSetScheduler2;
    (((root + 3)->childs)+2)->callback = execMenuActionSetScheduler3;
    (((root + 3)->childs)+3)->callback = execMenuActionSetScheduler4;

    //(root + 4) -> Check Sensors
}

void UserInterface::update()
{
    if (millis() - lastDebugTime > 1000)
    {
        DPRINTLN_F("UserInterface:currentStatus(%d):", this->currentStatus);
        lastDebugTime = millis();
    }
    switch (this->currentStatus)
    {
    case USER_INTERFACE_STATUS_OFF:
    {

        break;
    }
    case USER_INTERFACE_STATUS_USING_MENU:
    {
        break;
    }

    case USER_INTERFACE_STATUS_STANDBY:
    {
        break;
    }
    case USER_INTERFACE_EXEC_ACTION:
    {
        this->currentMenuAction->update();
        break;
    }
    }
}
#pragma endregion

#pragma region Actions

void UserInterface::pushTime(byte hours, byte minutes, byte seconds)
{
    //DPRINTLN_F("UserInterface::pushTime(%d,%d,%d):currentStatus=%d", hours, minutes, seconds, this->currentStatus);

    if (this->currentStatus == USER_INTERFACE_STATUS_STANDBY)
    {
        this->showIdleScreen(hours, minutes, seconds);
    }
}

#pragma endregion

#pragma region Callbacks

void UserInterface::onKeyUp(int key, void *caller_ptr)
{
    DPRINTLN_F("UserInterface::onKeyUp(%d)", key);
    UserInterface *me = static_cast<UserInterface *>(caller_ptr);

    me->lcd->resetLightOnTimeout();
    switch (me->currentStatus)
    {
    case USER_INTERFACE_STATUS_OFF:
    {
        me->lcd->wakeup();
        break;
    }
    case USER_INTERFACE_STATUS_STANDBY:
    {
        if (key == Keys::KEY_ENTER)
        {
            me->currentStatus = USER_INTERFACE_STATUS_USING_MENU;
            me->menu->reset();
            me->processMenu(Keys::KEY_NO_KEY);
        }
        break;
    }
    case USER_INTERFACE_STATUS_USING_MENU:
    {
        me->processMenu(key);
        break;
    }
    case USER_INTERFACE_EXEC_ACTION:
    {
        me->processAction(key);
    }
    }
}

void UserInterface::onKeyDown(int key, void *caller_ptr)
{
}

void UserInterface::onLcdSleep(void *caller_ptr)
{
    UserInterface *me = static_cast<UserInterface *>(caller_ptr);
    me->currentStatus = USER_INTERFACE_STATUS_OFF;
    DPRINTLN("UserInterface::onLcdSleep");

    me->lcd->clear();
}

void UserInterface::onLcdWakeup(void *caller_ptr)
{
    UserInterface *me = static_cast<UserInterface *>(caller_ptr);
    me->currentStatus = USER_INTERFACE_STATUS_STANDBY;

    DPRINTLN("UserInterface::onLcdWakeup");
}

void UserInterface::onActionComplete(void *caller_ptr, byte tag)
{
    Scheduler* sch = nullptr;

    UserInterface *me = static_cast<UserInterface *>(caller_ptr);
    switch (tag)
    {
        case menuTag::setDateTime:
            me->menuActionSetDateTime->getSettedDateTime(&(me->menuDateTime));
            if(me->onSetClock != NULL){
                me->onSetClock(&(me->menuDateTime), me->callerCallbackInstance);
            }
        break;
        case menuTag::setScheduler1:
            sch = me->scheduler1;
            break;
        case menuTag::setScheduler2:
            sch = me->scheduler2;
            break;
        case menuTag::setScheduler3:
            sch = me->scheduler3;
            break;
        case menuTag::setScheduler4:
            sch = me->scheduler4;
            break;
        break;
    }

    if(sch != nullptr)
    {
        byte hour;
        byte minute;
        byte duration;
        me->menuActionSetDateTime->getSettedScheduler(hour, minute, duration);
        sch->hour = hour;
        sch->minute = minute;
        sch->duration = duration;
    }
    me->currentStatus = USER_INTERFACE_STATUS_STANDBY;
}

void UserInterface::onActionCanceled(void *caller_ptr, byte tag)
{
    UserInterface *me = static_cast<UserInterface *>(caller_ptr);
    me->currentStatus = USER_INTERFACE_STATUS_STANDBY;
}

void UserInterface::execMenuOpen(void *caller_ptr, byte index)
{
    UserInterface *me = static_cast<UserInterface *>(caller_ptr);
    me->wprogram->open();
}

void UserInterface::execMenuClose(void *caller_ptr, byte index)
{
    UserInterface *me = static_cast<UserInterface *>(caller_ptr);
    me->wprogram->abort();
}

void UserInterface::execMenuActionSetDateTime(void *caller_ptr, byte index)
{
    UserInterface *me = static_cast<UserInterface *>(caller_ptr);
    me->currentStatus = USER_INTERFACE_EXEC_ACTION;
    me->menuActionSetDateTime->initSetDateTime();
    me->currentMenuAction = me->menuActionSetDateTime;
    me->currentMenuAction->reset(menuTag::setDateTime);
}

void UserInterface::execMenuActionSetScheduler1(void *caller_ptr, byte index)
{
    UserInterface *me = static_cast<UserInterface *>(caller_ptr);
    me->currentStatus = USER_INTERFACE_EXEC_ACTION;

    me->menuActionSetDateTime->initSetScheduler();
    me->currentMenuAction = me->menuActionSetDateTime;
    me->currentMenuAction->reset(menuTag::setScheduler1);
}

void UserInterface::execMenuActionSetScheduler2(void *caller_ptr, byte index)
{
    UserInterface *me = static_cast<UserInterface *>(caller_ptr);
    me->currentStatus = USER_INTERFACE_EXEC_ACTION;

    me->menuActionSetDateTime->initSetScheduler();
    me->currentMenuAction = me->menuActionSetDateTime;
    me->currentMenuAction->reset(menuTag::setScheduler2);
}

void UserInterface::execMenuActionSetScheduler3(void *caller_ptr, byte index)
{
    UserInterface *me = static_cast<UserInterface *>(caller_ptr);
    me->currentStatus = USER_INTERFACE_EXEC_ACTION;

    me->menuActionSetDateTime->initSetScheduler();
    me->currentMenuAction = me->menuActionSetDateTime;
    me->currentMenuAction->reset(menuTag::setScheduler3);
}

void UserInterface::execMenuActionSetScheduler4(void *caller_ptr, byte index)
{
    UserInterface *me = static_cast<UserInterface *>(caller_ptr);
    me->currentStatus = USER_INTERFACE_EXEC_ACTION;

    me->menuActionSetDateTime->initSetScheduler();
    me->currentMenuAction = me->menuActionSetDateTime;
    me->currentMenuAction->reset(menuTag::setScheduler4);
}

#pragma endregion

#pragma region Private

void UserInterface::showIdleScreen(byte hours, byte minutes, byte seconds)
{
    //DPRINTLN_F("UserInterface::showIdleScreen(%d,%d,%d):currentStatus=%d", hours, minutes, seconds, this->currentStatus);

    char line[16 + 1] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    lcd->writeLn(1, 0, "                \0");
    lcd->writeLn(0, 0, "                \0");

    sprintf(line, "Good %s", (hours > 6) && (hours < 12) ? "morning" : ((hours > 12) && (hours < 20) ? "afternoon" : "night"));
    lcd->writeLn(0, 0, line);

    sprintf(line, "%02d:%02d:%02d", hours, minutes, seconds);
    lcd->writeLn(1, 0, line);
}

void UserInterface::showMenuScreen(MenuItem *item)
{
    //DPRINTLN_F("UserInterface::showMenuScreen{%d,%s}:currentStatus=%d", item.title.c_str(), this->currentStatus);

    char line[16 + 1] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    lcd->writeLn(1, 0, "                \0");
    lcd->writeLn(0, 0, "                \0");

    snprintf(line, 16 + 1, "%s%c", item->title.c_str(), (item->hasChildren() ? 0x7E : 0x00));
    lcd->writeLn(0, 0, line);
}

void UserInterface::processMenu(Keys pressedKey)
{
    DPRINTLN_F("UserInterface::processMenu(%d)", pressedKey);
    switch (pressedKey)
    {
    case Keys::KEY_ENTER:
        menu->enter();
        break;
    case Keys::KEY_BACK:
        if (menu->isCurrentItemRoot())
        {
            this->currentStatus = USER_INTERFACE_STATUS_STANDBY;
            return;
        }
        else
        {
            menu->reset();
        }
        break;
    case Keys::KEY_UP:
        menu->moveNext();
        break;
    case Keys::KEY_DOWN:
        menu->movePrevious();
        break;
    };
    MenuItem *item = this->menu->getCurrentItem();
    this->showMenuScreen(item);
}

void UserInterface::processAction(Keys pressedKey)
{
    DPRINTLN_F("UserInterface::processSettingDateTime(%d)", pressedKey);
    switch (pressedKey)
    {
    case Keys::KEY_ENTER:
        currentMenuAction->keyEnter();
        break;
    case Keys::KEY_BACK:
        //cancel
        currentMenuAction->keyBack();
        break;
    case Keys::KEY_UP:
        currentMenuAction->keyUp();
        break;
    case Keys::KEY_DOWN:
        currentMenuAction->keyDown();
    };
}

void UserInterface::setMenuCallbacks()
{
   
}
#pragma endregion