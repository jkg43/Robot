#pragma once
#include "types.h"


void addConditionalEvent(unsigned long delayMs, Condition cond, EventName name, int arg1, int arg2);
void addConditionalEvent(unsigned long delayMs, Condition cond, EventName name, int arg);
void addConditionalEvent(unsigned long delayMs, Condition cond, EventName name);
void addEvent(unsigned long delayMs,EventName name, int arg1, int arg2);
void addEvent(unsigned long delayMs,EventName name, int arg);
void addEvent(unsigned long delayMs,EventName name);
void processEvents(Condition cond);