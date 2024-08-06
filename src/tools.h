#pragma once
#include "types.h"


void pwmSetup(PWMChannel channel, int pin, int frequency, int resolutionBits);
Condition getSensorCondition(Side s);
Side convertSideToDontEnable(Side s);
void moveArmTimed(int delay, int prevPos, int newPos, int timeMS);
int sign(int val);