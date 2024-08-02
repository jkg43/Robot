#include <Arduino.h>
#include "sensors.h"
#include "types.h"
#include "actions.h"
#include "tools.h"
#include "menus.h"
#include "globals.h"
#include "defns.h"
#include "event.h"

extern Globals g;

#define SENSOR_COOLDOWN_MS 50

unsigned long prevLeftSensorTime = 0;
unsigned long prevRightSensorTime = 0;

unsigned long leftSensorActivationStartTime = 0;
unsigned long rightSensorActivationStartTime = 0;
bool leftSensorActive = false;
bool rightSensorActive = false;


void updateSensors()
{

    if(leftSensorActive && millis() - leftSensorActivationStartTime >= param(SENSOR_COOLDOWN_TIME))
    {
        prevLeftSensorTime = millis();
        addEvent(0,E_PROCESS_EVENTS,C_LEFT_TAPE_SENSOR);
        addEvent(0,E_LED_ON);
        addEvent(500,E_LED_OFF);
    }

    if(rightSensorActive && millis() - rightSensorActivationStartTime >= param(SENSOR_COOLDOWN_TIME))
    {
        prevRightSensorTime = millis();
        addEvent(0,E_PROCESS_EVENTS,C_RIGHT_TAPE_SENSOR);
        // addEvent(0,E_LED_ON);
        // addEvent(500,E_LED_OFF);
    }




    // int leftTapeVal = analogRead(LEFT_TAPE_SENSOR_PIN);
    // int rightTapeVal = analogRead(RIGHT_TAPE_SENSOR_PIN);

    // int threshold = param(TAPE_SENSOR_THRESHOLD);

    // if(millis() - prevLeftSensorTime >= SENSOR_COOLDOWN_MS)
    // {
    //     if(leftTapeVal >= threshold)
    //     {
    //         prevLeftSensorTime = millis();
    //         processEvents(C_LEFT_TAPE_SENSOR);
    //         // addEvent(0,E_LED_ON);
    //         // addEvent(500,E_LED_OFF);
    //     }

    // }

    // if(millis() - prevRightSensorTime >= SENSOR_COOLDOWN_MS)
    // {
    //     if(rightTapeVal >= threshold)
    //     {
    //         prevRightSensorTime = millis();
    //         processEvents(C_RIGHT_TAPE_SENSOR);
    //         // addEvent(0,E_LED_ON);
    //         // addEvent(500,E_LED_OFF);
    //     }

    // }
    
}


void IRAM_ATTR left_tape_sensor_interrupt()
{

    if(millis() - prevLeftSensorTime >= param(SENSOR_COOLDOWN_TIME))
    {
        prevLeftSensorTime = millis();
        addEvent(0,E_PROCESS_EVENTS,C_LEFT_TAPE_SENSOR);
    }

    // if(millis() - prevLeftSensorTime < SENSOR_COOLDOWN_MS)
    // {
    //     return;
    // }

    // if(digitalRead(LEFT_TAPE_SENSOR_PIN) == LOW)
    // {
    //     leftSensorActive = false;
    // }
    // else
    // {
    //     leftSensorActive = true;
    //     leftSensorActivationStartTime = millis();
    // }


    // if(millis() - prevLeftSensorTime >= SENSOR_COOLDOWN_MS)
    // {
    //     prevLeftSensorTime = millis();
    //     addEvent(0,E_PROCESS_EVENTS,C_LEFT_TAPE_SENSOR);
    //     addEvent(0,E_LED_ON);
    //     addEvent(500,E_LED_OFF);
    // }
}

void IRAM_ATTR right_tape_sensor_interrupt()
{

    if(millis() - prevRightSensorTime >= param(SENSOR_COOLDOWN_TIME))
    {
        prevRightSensorTime = millis();
        addEvent(0,E_PROCESS_EVENTS,C_RIGHT_TAPE_SENSOR);
    }

    // if(millis() - prevRightSensorTime < SENSOR_COOLDOWN_MS)
    // {
    //     return;
    // }

    // if(digitalRead(RIGHT_TAPE_SENSOR_PIN) == LOW)
    // {
    //     rightSensorActive = false;
    // }
    // else
    // {
    //     rightSensorActive = true;
    //     rightSensorActivationStartTime = millis();
    // }


    // if(digitalRead(LEFT_TAPE_SENSOR_PIN) == LOW)
    // {
    //     return;
    // }
    // if(millis() - prevRightSensorTime >= SENSOR_COOLDOWN_MS)
    // {
    //     prevRightSensorTime = millis();
    //     addEvent(0,E_PROCESS_EVENTS,C_RIGHT_TAPE_SENSOR);
    // }
}



//checks the state of a button and updates its internal state accordingly
void checkButton(int b)
{
    unsigned long currentTime = millis();
    unsigned long elapsedTime = currentTime - g.prevButtonTime[b];
    if(elapsedTime < param(PRESS_DELAY))
    {
        return;
    }
    bool active = !digitalRead(buttonPins[b]);
    if(!g.buttonPressed[b] && active)
    {
        g.buttonPressed[b] = true;
        g.prevButtonTime[b] = currentTime;
        handlePress(b,false);
        return;
    }
    else if (g.buttonPressed[b] && !active)
    {
        g.buttonPressed[b] = false;
        g.prevButtonTime[b] = currentTime;
    }
    if(b == SELECT_BUTTON || elapsedTime < param(REPEAT_INITIAL))
    {
        return;
    }
    if(g.buttonPressed[b])
    {
        if(currentTime - g.prevRepeatTime[b] >= param(REPEAT_PERIOD))
        {
            g.prevRepeatTime[b] = currentTime;
            handlePress(b,true);
        }
    }
}

