#pragma once
#include "types.h"

struct Globals
{
    //WHEN ADDING THINGS: MAKE SURE TO RESET THEM IN clearEvents()
    bool enableLed = false;
    bool fullMotorOn = false;
    int currMenuIndex = 0;
    int mainMenuIndex = 0;
    Menu currSubMenu = SM_MAIN;
    bool inMenu = false;

    bool motorOn = false;
    bool bridgeMotorOn = false;

    unsigned long timer = 0;
    unsigned long timerStart = 0;

    int numCheesePlates = 0;
    int numSaladPlates = 0;


    // int armPos = 90;

    bool handoffReady = false;

    int stage = 0;

    //BUTTON VARS
    unsigned long prevButtonTime[3] = {0,0,0};
    bool buttonPressed[3] = {false,false,false};
    unsigned long prevRepeatTime[3] = {0,0,0};
    #if ROBOT == 3
    bool debugPinOn = false;
    #endif
    
    
};


