#include <map>
#include <Preferences.h>
#include "tools.h"
#include "menus.h"
#include "types.h"
#include "globals.h"
#include "defns.h"
#include "event.h"

extern Globals g;
extern Preferences pref;


void pwmSetup(PWMChannel channel, int pin, int frequency, int resolutionBits)
{
    ledcSetup(channel,frequency,resolutionBits);
    ledcAttachPin(pin,channel);
}

Condition getSensorCondition(Side s)
{
    if(s == SIDE_LEFT)
    {
        return C_LEFT_TAPE_SENSOR;
    }
    if(s == SIDE_RIGHT)
    {
        return C_RIGHT_TAPE_SENSOR;
    }
    return C_NONE;
}

void moveArmTimed(int delay, int prevPos, int newPos, int timeMS)
{
    int delta = timeMS / param(TIMED_MOVE_DIVISIONS);
    for (int i = 1; i <= param(TIMED_MOVE_DIVISIONS); i++)
    {
        addEvent(delay + delta * i,E_SET_ARM_POS,prevPos + ((i * (newPos - prevPos)) / param(TIMED_MOVE_DIVISIONS)));
    }
    
}

int sign(int val)
{
    if(val > 0)
    {
        return 1;
    }
    else if(val < 0)
    {
        return -1;
    }
    return 0;
}