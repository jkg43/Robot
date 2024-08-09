#pragma once
#include <string>
#include "defns.h"


// no prefix for normal menus
// SM_ prefix for a menu that goes to a submenu
// A_ prefix for regular actions
// AM_ prefix for going to a menu
enum Menu
{
    PRESS_DELAY,
    REPEAT_INITIAL,
    REPEAT_PERIOD,
    MOTOR_SPEED,
    MOTOR_SPEED_SLOW,
    MOTOR_SPEED_FAST,
    SWITCH_MOTOR_DIR,
    LED_TEST,
    EVENT_TEST,
    MOTOR_TEST_TIME,
    MOTOR_TEST_ACTIVATE,
    MOTOR_TEST_FULL,
    CLEAR_PREF,
    COND_TEST,
    TAPE_SENSOR_THRESHOLD,
    TAPE_DRIVE_TEST,
    REVERSE_DRIVE_TIME,
    REVERSE_DRIVE_POWER,
    REVERSE_DRIVE_DELAY,
    REVERSE_DELAY_SLOW,
    REVERSE_POWER_SLOW,
    REVERSE_DELAY_FAST,
    REVERSE_POWER_FAST,
    FLIPPER_POS,
    SET_FLIPPER_POS,
    ARM_POS,
    SET_ARM_POS,
    ARM_TIMED,
    PUSHER_1_POS,
    SET_PUSHER_1_POS,
    PUSHER_2_POS,
    SET_PUSHER_2_POS,
    SKIP_DELAY,
    SENSOR_SIDE,
    SENSOR_COOLDOWN_TIME,
    NEXT_TAPE_TEST,
    SKIP_1_TEST,
    SKIP_2_TEST,
    ENDURANCE_TEST,
    BT_CONNECT,
    GRAB_FOOD,
    STAGES,
    SERVING_AREA_TIME,
    GOTO_SERVING_AREA,
    BURGER_SERVING_TIME,
    SERVE_BURGER,
    SEND_HANDOFF_SIGNAL,
    ARM_MOVE_TIME,
    TIMED_MOVE_DIVISIONS,
    DISABLE_PUSHER_1,
    DISABLE_PUSHER_2,
    LEFT_GRAB_ARM_POS,
    RIGHT_GRAB_ARM_POS,
    LEFT_GRAB_PUSHER_POS,
    RIGHT_GRAB_PUSHER_POS,
    DISABLE_ARM,
    PLATE_GRAB_ARM_POS_1,
    PLATE_GRAB_ARM_POS_2,
    PLATE_GRAB_PUSHER_POS,
    PLATE_GRAB_DELTA,
    BURGER_SERVE_ARM_POS,
    BURGER_SERVE_PUSHER_POS,
    PUSHER_LEFT_DEFAULT_STATE,
    PUSHER_RIGHT_DEFAULT_STATE,
    PLACE_FOOD_ARM_POS, 
    PLACE_FOOD_PUSHER_POS,
    CLEAR_EVENTS,
    SLOW_DRIVE_TEST,
    PUSHER_MOVE_TIME,
    BRIDGE_HOLD_TIME,
    BRIDGE_RAISE_TIME,
    HOLD_BRIDGE,
    RAISE_BRIDGE,
    HOLD_BRIDGE_POWER,
    RAISE_BRIDGE_POWER,
    SERVING_AREA_TIME_MULTIPLIER,
    FLIP_TIME,
    BRIDGE_TEST,
    BRIDGE_RAISE_HOLD,
    BRIDGE_RELEASE_TIME,
    RELEASE_BRIDGE,
    PREP_RUN,
    PLATE_GRAB_ARM_TIME,
    SALAD_BURGER,
    
    #if ROBOT == 1
    GRAB_PLATE,
    HANDOFF_DELTA,
    CHEESE_PLATE,
    SALAD_PLATE,
    #elif ROBOT == 2
    PLACE_FOOD,
    #elif ROBOT == 3
    DEBUG_PIN,
    ACTIVATE_DEBUG_PIN,
    #endif

    SM_MAIN,
    SM_DRIVE,
    SM_ARM,
    SM_PUSHERS,
    SM_SENSORS,
    SM_GRAB,
    SM_BT,
    SM_SEQUENCES,
    SM_ESP,
    SM_BRIDGE,
    SM_IMPORTANT,

};




enum EventName
{
    E_NONE,
    E_LED_ON,
    E_LED_OFF,
    //arg1 is the motor power, duty cycle out of 4096, can be negative
    E_MOTOR_PWM_ON,
    E_MOTOR_PWM_OFF,
    E_SWITCH_MOTOR_DIRECTION,
    //arg1 is degrees, 0 to 180
    E_SET_FLIPPER_POS,
    E_DISABLE_FLIPPER,
    //arg1 is degrees, -180 to 360
    E_SET_ARM_POS,
    //arg1 is degrees, -180 to 360
    //arg2 is the time in ms the move should take
    E_MOVE_ARM_TIMED,
    //arg1 is degrees, -180 to 360
    E_SET_PUSHER_RIGHT_POS,
    //arg1 is degrees, -180 to 360
    E_SET_PUSHER_LEFT_POS,
    //arg1 is the sequence id to activate
    //arg2 is an optional arg for the sequence
    E_ACTIVATE_SEQUENCE,
    E_DISABLE_PUSHER_1,
    E_DISABLE_PUSHER_2,
    E_DISABLE_ARM,
    //arg1 is the condition of events to activate
    E_PROCESS_EVENTS,
    //arg1 is a char to send via Bluetooth
    E_SEND_BT,
    E_HANDOFF_DONE,
    E_CLEAR_EVENTS,
    //arg1 is the Speed, either SPEED_FAST or SPEED_SLOW
    //arg2 is a multiplier
    E_SET_MOTOR_SPEED,
    E_START_TIMER,
    E_STOP_TIMER,
    //arg1 is the motor power, duty cycle out of 4096
    E_BRIDGE_PWM_ON,
    E_BRIDGE_PWM_OFF,
};


// C_ prefix is a normal condition
// D_ prefix is a condition for when a sequence is done
enum Condition
{
    C_NONE,
    C_LEFT_BUTTON,
    C_SELECT_BUTTON,
    C_RIGHT_BUTTON,
    C_LEFT_TAPE_SENSOR,
    C_RIGHT_TAPE_SENSOR,
    D_NEXT_TAPE,
    D_SKIP_1,
    D_SKIP_2,
    D_STOP,
    D_GRAB_FOOD,
    D_MOVE_1_GRAB,
    D_MOVE_2_GRAB,
    D_MOVE_1_PRE_GRAB,
    D_MOVE_2_PRE_GRAB,
    C_HANDOFF_READY,
    D_PLACE_FOOD,
    D_GRAB_PLATE,
    D_GOTO_SERVING,
    D_SERVE_BURGER,
    D_HANDOFF_DELTA,
    D_MAKE_BURGER,
    D_MAKE_CHEESE_PLATE,
    D_MAKE_SALAD,
    D_PREP_RUN,
};

enum Sequence
{
    S_NONE,
    //arg should be a Side indicating which side of sensor to use
    S_STOP_MOTOR,
    //arg should be a Side indicating which side of sensor to use
    S_NEXT_TAPE,
    //arg should be a Side indicating which side of sensor to use
    S_NEXT_TAPE_SKIP_1,
    //arg should be a Side indicating which side of sensor to use
    S_NEXT_TAPE_SKIP_2,
    S_ENDURANCE_TEST,
    //arg is the Side to grab from
    S_GRAB_FOOD,
    //arg is the Side to grab from
    S_PRE_GRAB_FOOD,
    //arg is the Side to grab from
    S_GRAB_FOOD_1,
    //arg is the Side to grab from
    S_GRAB_FOOD_2,
    S_GRAB_PLATE,
    S_GRAB_CONTINUOUS,
    //arg is the Side to detect and grab from
    S_MOVE_1_GRAB,
    //arg is the Side to detect and grab from
    S_MOVE_2_GRAB,
    //arg is the Side to detect and grab from
    S_MOVE_1_PRE_GRAB,
    //arg is the Side to detect and grab from
    S_MOVE_2_PRE_GRAB,
    S_PLACE_FOOD,
    //handles speed
    S_GOTO_SERVING_AREA,
    S_SERVE_BURGER,
    //arg should be either 1 or -1 to indicate the direction of delta 
    //  relative to current movement direction
    S_HANDOFF_DELTA,
    //arg should be a Side indicating which side of sensor to use
    S_TIMED_STOP,
    S_RAISE_BRIDGE,
    S_RELEASE_BRIDGE,
    S_PREP_RUN,


    S_STAGE_0,
    S_STAGE_1,
    S_STAGE_2,
    S_STAGE_3,
    S_STAGE_4,
    S_STAGE_5,
    S_STAGE_6,
    S_STAGE_7,
    S_STAGE_8,
    S_STAGE_9,
    S_CHEESE_PLATE_STAGE_0,
    S_CHEESE_PLATE_STAGE_1,
    S_CHEESE_PLATE_STAGE_2,
    S_CHEESE_PLATE_STAGE_3,
    S_SALAD_PLATE_STAGE_0,
    S_SALAD_PLATE_STAGE_1,
    S_SALAD_PLATE_STAGE_2,
    S_SALAD_PLATE_STAGE_3,
    S_SALAD_PLATE_STAGE_4,
};

enum BTCommands
{
    BT_LED = 'L',
    BT_HANDOFF_READY = 'H',
    BT_START_BURGER = 'B',
};


struct MenuItem
{
    std::string name;
    int val;
    int delta;
    int min;
    int max;
    bool isVariable;
};

struct Event
{
    unsigned long activationTime;
    int condition;
    int name;
    int arg1;
    int arg2;
    Event(unsigned long t, int c, int n, int a1, int a2) : activationTime(t), condition(c), name(n), arg1(a1), arg2(a2){};
    Event() : activationTime(0), condition(C_NONE), name(E_NONE), arg1(0), arg2(0){};
};

enum Button
{
    LEFT_BUTTON,
    SELECT_BUTTON,
    RIGHT_BUTTON
};

enum Side
{
    SIDE_LEFT = 0,
    SIDE_RIGHT = 1,
    SIDE_LEFT_DONT_ENABLE = 2,
    SIDE_RIGHT_DONT_ENABLE = 3,
};

enum Speed
{
    SPEED_FAST,
    SPEED_SLOW,
};

enum PWMChannel
{
    DRIVE_FWD_CHANNEL = 0,
    DRIVE_REV_CHANNEL = 1,
    FLIPPER_CHANNEL = 2,
    ARM_CHANNEL = 3,
    PUSHER_RIGHT_CHANNEL = 4,
    PUSHER_LEFT_CHANNEL = 5,
    BRIDGE_MOTOR_CHANNEL = 6,
};