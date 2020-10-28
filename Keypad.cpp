#include "headers\SerialDebug.h"
#include <Arduino.h>
#include "headers\Keypad.h"

Keypad::Keypad(byte pin)
{
    inputPin = pin;
}

void Keypad::begin()
{
    readKeys[0] = KEY_NO_KEY;
    readKeys[1] = KEY_NO_KEY;
    readKeys[2] = KEY_NO_KEY;
    lastPressedKey = KEY_NO_KEY;
    previousDebounceTime = 0;
}

void Keypad::update()
{
    unsigned long currentTime = millis();
    Keys detectedKey;

    if ((currentTime - previousDebounceTime) > DEBOUCE_KEY_VALUE)
    {
        previousDebounceTime = currentTime;

        int pin_value = analogRead(inputPin);
        //DPRINTLN_F("Keypad::update(%d)", pin_value);

        if (((KEY_ENTER_VALUE - 50) < pin_value) && (pin_value < (KEY_ENTER_VALUE + 50)))
        {
            //0 - 100
            detectedKey = KEY_ENTER;
        }
        else if (((KEY_UP_VALUE - 50) < pin_value) && (pin_value < (KEY_UP_VALUE + 50)))
        {
            //110 - 210
            detectedKey = KEY_UP;
        }
        else if (((KEY_DOWN_VALUE - 50) < pin_value) && (pin_value < (KEY_DOWN_VALUE + 50)))
        {
            //275 - 375
            detectedKey = KEY_DOWN;
        }
        else if (((KEY_BACK_VALUE - 50) < pin_value) && (pin_value < (KEY_BACK_VALUE + 50)))
        {
            //420 - 520
            detectedKey = KEY_BACK;
        }
        else
        {
            detectedKey = KEY_NO_KEY;
        }

        readKeys[keyReadingCount++] = detectedKey;
    }

    if (keyReadingCount > 2) //if we have 3 reading
    {
        keyReadingCount = 0;
        if ((readKeys[0] == readKeys[1]) && (readKeys[1] == readKeys[2]))
        {
            //DPRINTLN_F("Keypad::update([%d][%d][%d])", readKeys[0], readKeys[1], readKeys[2]);

            if (lastPressedKey != detectedKey)
            {
                if (lastPressedKey == KEY_NO_KEY)
                {
                    if (onKeyDown != NULL)
                    {
                        onKeyDown(detectedKey, callerCallbackInstance);
                    }
                }
                else if (detectedKey == KEY_NO_KEY)
                {
                    if (onKeyUp != NULL)
                    {
                        onKeyUp(lastPressedKey, callerCallbackInstance);
                    }
                }
                lastPressedKey = detectedKey;
            }
        }
    }
}
