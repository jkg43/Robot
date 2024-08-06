#include <list>
#include <Arduino.h>
#include "event.h"
#include "types.h"
#include "actions.h"
#include "defns.h"
#include "globals.h"

using namespace std;

extern list<Event> events;
extern Globals g;

//store events in order in the events list, with earlier elements having an earlier activation time
void addConditionalEvent(unsigned long delayMs, Condition cond, EventName name, int arg1, int arg2)
{
    unsigned long activationTime = millis() + delayMs;

    size_t max = events.size();
    if(max == 0)
    {
        events.emplace_front(activationTime,cond,name,arg1,arg2);
        return;
    }

    list<Event>::iterator it = events.begin();

    for(size_t i = 0; i < max; i++)
    {
        if((*it).activationTime > activationTime)
        {
            events.emplace(it,activationTime,cond,name,arg1,arg2);
            return;
        }
        advance(it,1);
    }

    events.emplace_back(activationTime,cond,name,arg1,arg2);
}

void addConditionalEvent(unsigned long delayMs, Condition cond, EventName name, int arg)
{
    addConditionalEvent(delayMs,cond,name,arg,0);
}


void addConditionalEvent(unsigned long delayMs, Condition cond, EventName name)
{
    addConditionalEvent(delayMs,cond,name,0,0);
}


void addEvent(unsigned long delayMs, EventName name, int arg1, int arg2)
{
    addConditionalEvent(delayMs,C_NONE,name,arg1,arg2);
}

void addEvent(unsigned long delayMs, EventName name, int arg)
{
    addConditionalEvent(delayMs,C_NONE,name,arg);
}

void addEvent(unsigned long delayMs,EventName name)
{
    addEvent(delayMs,name,0);
}

void clearEvents()
{
    events.clear();
    ledcWrite(PUSHER_LEFT_CHANNEL,0);
    ledcWrite(PUSHER_RIGHT_CHANNEL,0);
    ledcWrite(ARM_CHANNEL,0);
    ledcWrite(DRIVE_FWD_CHANNEL,0);
    ledcWrite(DRIVE_REV_CHANNEL,0);
    ledcWrite(FLIPPER_CHANNEL,0);
    g.enableLed = false;
    g.motorOn = false;
    g.fullMotorOn = false;
    g.handoffReady = false;
    g.timer = 0;
    g.timerStart = 0;
    
    
}

//trigger and remove any events whose activation time has passed
void processEvents(Condition cond)
{
    unsigned long currentTime = millis();
    list<Event>::iterator it = events.begin();

    size_t max = events.size();

    for(size_t i = 0; i < max; i++)
    {
        if((*it).name == E_CLEAR_EVENTS)
        {
            clearEvents();
            return;
        }
        if((*it).activationTime <= currentTime)
        {
            if((*it).condition == cond)
            {
                activateEvent((EventName)(*it).name, (*it).arg1, (*it).arg2);
                it = events.erase(it);
                continue;
            }
            else
            {
                advance(it,1);
            }
        }
        else
        {
            break;
        }
    }

}