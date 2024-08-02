#pragma once
#include "BluetoothSerial.h"
#include "defns.h"

void initBT();
void updateBT();

#if ROBOT == 1
void connectBT();
#endif
