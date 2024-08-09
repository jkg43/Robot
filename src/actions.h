#pragma once
#include "types.h"


//BACKEND
void handlePress(int b, bool repeat);
void selectButton();
void leftButton();
void rightButton();
void activateEvent(EventName name, int arg1, int arg2);
void activateEvent(EventName name, int arg1);
void triggerMenuAction(Menu m);
void activateSequence(Sequence s);
void activateSequence(Sequence s, int arg);

//CUSTOM
void enable_pwm(PWMChannel fwd_channel,PWMChannel rev_channel,int pwmVal);
void set_servo(int channel, int degrees);
void set_servo_strong(int channel, int degrees);