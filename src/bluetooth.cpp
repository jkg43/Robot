#include "bluetooth.h"
#include "BluetoothSerial.h"
#include "defns.h"
#include "event.h"
#include "globals.h"

extern Globals g;

BluetoothSerial BTSerial;
BTAddress otherAddr;


String ROBOT_1_ADDR = "64:B7:08:95:F8:42";
String ROBOT_2_ADDR = "64:B7:08:9C:5D:EA";
String ROBOT_3_ADDR = "64:B7:08:9D:69:EE";


void initBT()
{

    BTSerial.setTimeout(1);

    #if ROBOT == 1

    BTSerial.begin("ROBOT 1 BT TEAM 13",true);

    otherAddr = BTAddress(ROBOT_2_ADDR);

    connectBT();

    #elif ROBOT == 2

    BTSerial.begin("ROBOT 2 BT TEAM 13",false);

    otherAddr = BTAddress(ROBOT_1_ADDR);

    #endif
}

/*
BT Protocol:
Messages are Strings terminated in a newline character
The first character indicates the command
Any remaining characters are data

*/

#if ROBOT == 1

void connectBT()
{
    if(!BTSerial.connected())
    {
        BTSerial.connect(otherAddr);
    }
}

#endif

void updateBT()
{

    String currentData = BTSerial.readStringUntil('\n');

    char command = currentData.charAt(0);
    String data = currentData.substring(1);

    switch(command)
    {
        case BT_LED:
            addEvent(0,E_LED_ON);
            addEvent(1000,E_LED_OFF);
            break;
        case BT_HANDOFF_READY:
            g.handoffReady = true;
            processEvents(C_HANDOFF_READY);
            break;
    }



}
