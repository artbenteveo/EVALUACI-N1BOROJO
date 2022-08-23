#include <Arduino.h>
#include "task3.h"
#include "events.h"

static bool compareKeys(BUTTONS *pSecret, BUTTONS *pKey)
{
    bool correct = true;
    for (uint8_t i = 0; i < 5; i++)
    {
        if (pSecret[i] != pKey[i])
        {
            correct = false;
            break;
        }
    }

    return correct;
}

void task3()
{

    enum class TaskStates
    {
        INIT,
        WAIT_CONFIG,
        MODOLENTO,
        MODOMEDIO,
        MODORAPIDO,
        PAPAGADO,
        PENCENDIDO,

    };
    static TaskStates taskState = TaskStates::INIT;
    //
    static uint32_t lasTime;
    static constexpr uint32_t INTERVAL1 = 500;
    static constexpr uint32_t INTERVAL2 = 250;
    static constexpr uint32_t INTERVAL3 = 125;

    static constexpr uint8_t ledRed = 14;
    static bool ledStatus = false;
    static bool estabaenPAPAGADO = false;

    static BUTTONS secret[5] = {BUTTONS::UP_BTN, BUTTONS::UP_BTN,
                                BUTTONS::DOWN_BTN, BUTTONS::DOWN_BTN,
                                BUTTONS::UP_BTN};

    static BUTTONS disarmKey[5] = {BUTTONS::NONE};
    static uint8_t keyCounter = 0;

    switch (taskState)
    {
    case TaskStates::INIT:
    {

        pinMode(ledRed, OUTPUT);
        digitalWrite(ledRed, LOW);

        taskState = TaskStates::MODOLENTO;
        break;
    }
    case TaskStates::MODOLENTO:
    {

        uint32_t currentTime = millis();
        if ((currentTime - lasTime) >= INTERVAL1)
        {
            lasTime = currentTime;
            digitalWrite(ledRed, ledStatus);
            ledStatus = !ledStatus;
            if (buttonEvt.trigger == true)
            {
                buttonEvt.trigger = false;
                if (buttonEvt.whichButton == BUTTONS::UP_BTN)
                {
                    taskState = TaskStates::PAPAGADO;
                }
                else if (buttonEvt.whichButton == BUTTONS::DOWN_BTN)
                {
                    taskState = TaskStates::MODOMEDIO;
                }
            }
        }

        break;
    }
    case TaskStates::PAPAGADO:
    {
        digitalWrite(ledRed, ledStatus);
        ledStatus = false;
        if (buttonEvt.trigger == true)
        {
            buttonEvt.trigger = false;
            if (buttonEvt.whichButton == BUTTONS::UP_BTN)
            {
                taskState = TaskStates::MODOLENTO;
            }
            else if (buttonEvt.whichButton == BUTTONS::DOWN_BTN)
            {
                estabaenPAPAGADO = true;
                taskState = TaskStates::MODORAPIDO;
            }
        }

        break;
    }

    case TaskStates::MODOMEDIO:
    {
        uint32_t currentTime = millis();
        if ((currentTime - lasTime) >= INTERVAL2)
        {
            lasTime = currentTime;
            digitalWrite(ledRed, ledStatus);
            ledStatus = !ledStatus;
            if (buttonEvt.trigger == true)
            {
                buttonEvt.trigger = false;
                if (buttonEvt.whichButton == BUTTONS::UP_BTN)
                {
                    taskState = TaskStates::PENCENDIDO;
                }
                else if (buttonEvt.whichButton == BUTTONS::DOWN_BTN)
                {
                    taskState = TaskStates::MODOLENTO;
                }
            }
        }

        break;
    }
    case TaskStates::PENCENDIDO:
    {
        digitalWrite(ledRed, ledStatus);
        ledStatus = true;
        if (buttonEvt.trigger == true)
        {
            buttonEvt.trigger = false;
            if (buttonEvt.whichButton == BUTTONS::UP_BTN)
            {
                taskState = TaskStates::MODOMEDIO;
            }
            else if (buttonEvt.whichButton == BUTTONS::DOWN_BTN)
            {
                estabaenPAPAGADO = false;
                taskState = TaskStates::MODORAPIDO;
            }
        }

        break;
    }
    case TaskStates::MODORAPIDO:
    {
        uint32_t currentTime = millis();
        if ((currentTime - lasTime) >= INTERVAL3)
        {
            lasTime = currentTime;
            digitalWrite(ledRed, ledStatus);
            ledStatus = !ledStatus;
        }
        if (buttonEvt.trigger == true)
        {
            buttonEvt.trigger = false;
            disarmKey[keyCounter] = buttonEvt.whichButton;
            keyCounter++;
            if (keyCounter == 5)
            {
                keyCounter = 0;
                if (compareKeys(secret, disarmKey) == true)
                {

                    Serial.print("salir de modo rápido\n");

                    if (estabaenPAPAGADO == true)
                    {
                        taskState = TaskStates::PAPAGADO;
                    }
                    else
                    {
                        taskState = TaskStates::PENCENDIDO;
                    }
                }
                else
                {
                    Serial.print("código equivocado\n");
                }
            }
        }

        break;
    }

    default:
    {
        break;
    }
    }
}
