#include "menus.h"
#include "defns.h"
#include "globals.h"
#include "types.h"
#include "Preferences.h"
#include <unordered_map>
#include <vector>

using namespace std;

extern Globals g;
extern Preferences pref;

// Param ID, then: Name, Default Value, Increment, Min, Max,[Variable]
unordered_map<Menu,MenuItem> menus = {
    {PRESS_DELAY,{"Press Delay",25,5,10,500,true}},
    {REPEAT_INITIAL,{"Repeat Delay",500,25,50,2000,true}},
    {REPEAT_PERIOD,{"Repeat Period",100,10,20,500,true}},
    {LED_TEST,{"LED Test"}},
    {MOTOR_SPEED,{"Motor Speed",0,16,-4096,4096,true}},
    {MOTOR_SPEED_SLOW,{"Motor Speed Slow",0,8,-4096,4096,true}},
    {SWITCH_MOTOR_DIR,{"Switch Motor Direction"}},
    {EVENT_TEST,{"Event Test"}},
    {MOTOR_TEST_TIME,{"Motor Time Test Seconds",2,1,1,10,true}},
    {MOTOR_TEST_ACTIVATE,{"Activate Motor Test"}},
    {MOTOR_TEST_FULL,{"Activate Full Motor Test"}},
    {CLEAR_PREF,{"Clear Preferences"}},
    {COND_TEST,{"Conditional Test"}},
    {TAPE_SENSOR_THRESHOLD,{"Sensor Threshold",128,1,0,255,true}},
    {TAPE_DRIVE_TEST,{"Tape Drive Test"}},
    {REVERSE_DRIVE_TIME,{"Reverse Drive Time MS",50,10,10,5000,true}},
    {REVERSE_DRIVE_POWER,{"Reverse Drive Power (%)",50,2,0,100,true}},
    {REVERSE_DRIVE_DELAY,{"Reverse Drive Delay MS",50,10,10,5000,true}},
    {FLIPPER_POS,{"Flipper Pos",0,10,0,180,true}},
    {SET_FLIPPER_POS,{"Set Flipper Pos"}},
    {ARM_POS,{"Arm Pos",0,2,-180,360,true}},
    {SET_ARM_POS,{"Set Arm Pos"}},
    {ARM_TIMED,{"Move Arm Timed"}},
    {PUSHER_1_POS,{"Pusher 1 Pos",0,2,-180,360,true}},
    {SET_PUSHER_1_POS,{"Set Pusher 1 Pos"}},
    {PUSHER_2_POS,{"Pusher 2 Pos",0,2,-180,360,true}},
    {SET_PUSHER_2_POS,{"Set Pusher 2 Pos"}},
    {SKIP_DELAY,{"Tape Skip Delay",500,10,100,5000,true}},
    {SENSOR_SIDE,{"Sensor Side",0,1,0,1,true}},
    {SENSOR_COOLDOWN_TIME,{"Sensor Cooldown Time MS",10,1,0,5000,true}},
    {NEXT_TAPE_TEST,{"Next Tape Test"}},
    {SKIP_1_TEST,{"Skip 1 Test"}},
    {SKIP_2_TEST,{"Skip 2 Test"}},
    {ENDURANCE_TEST,{"Endurance Test"}},
    {BT_CONNECT,{"Connect BT"}},
    {GRAB_FOOD,{"Grab Food"}},
    {STAGES,{"Stages"}},
    {SERVING_AREA_TIME,{"Serving Area Time MS",1000,25,0,10000,true}},
    {GOTO_SERVING_AREA,{"Go to Serving Area"}},
    {BURGER_SERVING_TIME,{"Burger Serving Time MS",5000,100,0,20000,true}},
    {SERVE_BURGER,{"Serve Burger"}},
    {ARM_MOVE_TIME,{"Arm Move Time MS",2000,25,0,10000,true}},
    {SEND_HANDOFF_SIGNAL,{"Send Handoff Signal"}},
    {TIMED_MOVE_DIVISIONS,{"Timed Move Divisions",20,1,0,100,true}},
    {DISABLE_PUSHER_1,{"Disable Pusher 1"}},
    {DISABLE_PUSHER_2,{"Disable Pusher 2"}},
    {LEFT_GRAB_ARM_POS,{"Left Grab Arm Pos",60,2,0,180,true}},
    {RIGHT_GRAB_ARM_POS,{"Right Grab Arm Pos",120,2,0,180,true}},
    {LEFT_GRAB_PUSHER_POS,{"Left Grab Pusher Pos",120,2,0,180,true}},
    {RIGHT_GRAB_PUSHER_POS,{"Right Grab Pusher Pos",120,2,0,180,true}},
    {DISABLE_ARM,{"Disable Arm"}},
    {PLATE_GRAB_ARM_POS_1,{"1 Plate Grab Arm Pos",90,2,0,180,true}},
    {PLATE_GRAB_ARM_POS_2,{"2 Plate Grab Arm Pos",90,2,0,180,true}},
    {PLATE_GRAB_PUSHER_POS,{"Plate Grab Pusher Pos",90,2,0,180,true}},
    {PLATE_GRAB_DELTA,{"Plate Grab Delta",-10,1,-180,180,true}},
    {BURGER_SERVE_ARM_POS,{"Burger Serve Arm Pos",90,2,0,180,true}},
    {BURGER_SERVE_PUSHER_POS,{"Burger Serve Pusher Pos",90,2,0,180,true}},
    {PUSHER_RIGHT_DEFAULT_STATE,{"Pusher 1 Default Pos",90,2,0,180,true}},
    {PUSHER_LEFT_DEFAULT_STATE,{"Pusher 2 Default Pos",90,2,0,180,true}},
    {PLACE_FOOD_ARM_POS,{"Place Food Arm Pos",90,2,0,180,true}},
    {PLACE_FOOD_PUSHER_POS,{"Place Food Pusher Pos",90,2,0,180,true}},
    {CLEAR_EVENTS,{"RESET"}},

    #if ROBOT == 1
    {GRAB_PLATE,{"Grab Plate"}},
    {HANDOFF_DELTA,{"Handoff Delta MS",0,5,-5000,5000,true}},
    {CHEESE_PLATE,{"Cheese Plate"}},
    {SALAD_PLATE,{"Salad Plate"}},

    #elif ROBOT == 2
    {PLACE_FOOD,{"Place Food"}},

    #elif ROBOT == 3
    {DEBUG_PIN,{"Debug Pin",1,1,1,100,true}},
    {ACTIVATE_DEBUG_PIN,{"Activate Debug Pin"}},
    #endif

    {SM_MAIN,{"Back to Main Menu"}},
    {SM_DRIVE,{"Drive Settings"}},
    {SM_ARM,{"Arm Settings"}},
    {SM_PUSHERS,{"Pusher Settings"}},
    {SM_SENSORS,{"Sensor Settings"}},
    {SM_GRAB,{"Grab Settings"}},
    {SM_BT,{"Bluetooth Settings"}},
    {SM_SEQUENCES,{"Sequences"}},
    {SM_ESP,{"ESP Settings"}},

}; //map is unordered, menu order depends on submenu vectors below

/*

TO ADD NEW MENU ITEMS:

variable: 
-create a new Menu in types.h
-add an entry to the map above, with variable set to true and the relevant arguments filled

action:
-create a new Menu in types.h
-add a new case to the switch statement in triggerAction in actions.cpp
-add an entry to the map above with the name of the actiont


TO CREATE A NEW EVENT:
-create a new EventName in types.h
-add a new case to the switch statement in activateEvent in actions.cpp


TO ADD A NEW SUBMENU:
-create a new Menu with the SM_ prefix
-add an entry to the menus map above to allow access to it
-add it into the subMenus map below and fill with the desired menus
-add code to access the submenu in triggerAction
-make sure to add it to the main submenu

*/





unordered_map<Menu,vector<Menu>> subMenus = {
    {SM_MAIN,{
        SM_DRIVE,
        SM_SENSORS,
        SM_ARM,
        SM_PUSHERS,
        SM_GRAB,
        SM_BT,
        SM_SEQUENCES,
        SM_ESP,

        CLEAR_EVENTS,
    }},
    {SM_DRIVE,{
        MOTOR_SPEED,
        SWITCH_MOTOR_DIR,
        MOTOR_SPEED_SLOW,
        MOTOR_TEST_TIME,
        MOTOR_TEST_ACTIVATE,
        MOTOR_TEST_FULL,
        REVERSE_DRIVE_DELAY,
        REVERSE_DRIVE_POWER,

        SM_MAIN,
    }},
    {SM_ARM,{
        ARM_POS,
        SET_ARM_POS,
        ARM_TIMED,
        DISABLE_ARM,
        ARM_MOVE_TIME,
        TIMED_MOVE_DIVISIONS,

        SM_MAIN,
    }},
    {SM_PUSHERS,{
        PUSHER_1_POS,
        SET_PUSHER_1_POS,
        PUSHER_RIGHT_DEFAULT_STATE,
        DISABLE_PUSHER_1,
        PUSHER_2_POS,
        SET_PUSHER_2_POS,
        PUSHER_LEFT_DEFAULT_STATE,
        DISABLE_PUSHER_2,

        SM_MAIN,
    }},
    {SM_SENSORS,{
        TAPE_SENSOR_THRESHOLD,
        SENSOR_SIDE,
        SKIP_DELAY,
        SENSOR_COOLDOWN_TIME,
        NEXT_TAPE_TEST,
        SKIP_1_TEST,
        SKIP_2_TEST,

        SM_MAIN,
    }},
    {SM_GRAB,{
        GRAB_FOOD,
        SENSOR_SIDE,
        LEFT_GRAB_ARM_POS,
        RIGHT_GRAB_ARM_POS,
        LEFT_GRAB_PUSHER_POS,
        RIGHT_GRAB_PUSHER_POS,
        BURGER_SERVING_TIME,
        BURGER_SERVE_ARM_POS,
        BURGER_SERVE_PUSHER_POS,
        SERVE_BURGER,

        #if ROBOT == 1
        GRAB_PLATE,
        PLATE_GRAB_ARM_POS_1,
        PLATE_GRAB_ARM_POS_2,
        PLATE_GRAB_PUSHER_POS,
        PLATE_GRAB_DELTA,
        #elif ROBOT == 2
        PLACE_FOOD,
        PLACE_FOOD_ARM_POS,
        PLACE_FOOD_PUSHER_POS,
        #endif

        SM_MAIN,
    }},
    {SM_BT,{
        BT_CONNECT,
        SEND_HANDOFF_SIGNAL,

        #if ROBOT == 1
        HANDOFF_DELTA,
        #endif

        SM_MAIN,
    }},
    {SM_SEQUENCES,{
        ENDURANCE_TEST,
        SERVING_AREA_TIME,
        GOTO_SERVING_AREA,
        STAGES,

        #if ROBOT == 1
        CHEESE_PLATE,
        SALAD_PLATE,
        #elif ROBOT == 2

        #endif

        SM_MAIN,
    }},
    {SM_ESP,{
        PRESS_DELAY,
        REPEAT_INITIAL,
        REPEAT_PERIOD,
        CLEAR_PREF,
        LED_TEST,
        EVENT_TEST,
        COND_TEST,

        SM_MAIN,
    }},



};


int param(Menu p)
{
  return menus.at(p).val;
}

int clampValToMenu(Menu p, int val)
{
  MenuItem& menu = menus.at(p);
  if(val > menu.max)
  {
    return menu.max;
  }
  if(val < menu.min)
  {
    return menu.min;
  }
  return val;
}

void setParam(Menu p, int val)
{
    MenuItem& menu = menus.at(p);
    menu.val = clampValToMenu(p,val);
    pref.putInt(menu.name.substr(0,15).c_str(),menu.val);
}

void changeParam(Menu p, int delta)
{
    setParam(p,param(p) + delta);
}

Menu currMenu()
{
    return subMenus.at(g.currSubMenu).at(g.currMenuIndex);
}

const MenuItem& menuItem(Menu m)
{
    return menus.at(m);
}

const MenuItem& currMenuItem()
{
    return menuItem(currMenu());
}

int numMenus()
{
    return subMenus.at(g.currSubMenu).size();
}

void nextMenu()
{
    g.currMenuIndex = (g.currMenuIndex + 1) % numMenus();
    if(g.currSubMenu == SM_MAIN)
    {
        g.mainMenuIndex = g.currMenuIndex;
    }
}

void prevMenu()
{
    g.currMenuIndex = (g.currMenuIndex - 1 + numMenus()) % numMenus();
    if(g.currSubMenu == SM_MAIN)
    {
        g.mainMenuIndex = g.currMenuIndex;
    }
}

std::string currSubMenuName()
{
    return menus.at(g.currSubMenu).name;
}