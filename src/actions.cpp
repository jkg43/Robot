#include <unordered_map>
#include <Preferences.h>
#include "actions.h"
#include "globals.h"
#include "tools.h"
#include "menus.h"
#include "event.h"
#include "defns.h"
#include "BluetoothSerial.h"
#include "macros.h"
#include "bluetooth.h"

extern Globals g;
extern Preferences pref;
extern BluetoothSerial BTSerial;

#define FWD_CHANNEL 0
#define REV_CHANNEL 1

//CUSTOM FUNCTIONALITY




void activateSequence(Sequence s, int arg)
{
    switch(s)
    {
        case S_STOP_MOTOR:
            addEvent(0,E_MOTOR_PWM_OFF);

            addEvent(param(REVERSE_DRIVE_DELAY)+1,E_MOTOR_PWM_ON,param(MOTOR_SPEED) * param(REVERSE_DRIVE_POWER) / -100);

            addConditionalEvent(param(REVERSE_DRIVE_DELAY)+1,getSensorCondition((Side)arg),E_MOTOR_PWM_OFF);

            addConditionalEvent(param(REVERSE_DRIVE_DELAY)+2,getSensorCondition((Side)arg),E_PROCESS_EVENTS,D_STOP);
            break;
        case S_NEXT_TAPE:
            addEvent(0,E_MOTOR_PWM_ON,param(MOTOR_SPEED));
            addConditionalEvent(param(SKIP_DELAY),getSensorCondition((Side)arg),E_ACTIVATE_SEQUENCE,S_STOP_MOTOR,arg);
            addConditionalEvent(param(SKIP_DELAY),getSensorCondition((Side)arg),E_PROCESS_EVENTS,D_NEXT_TAPE);
            break;
        case S_NEXT_TAPE_SKIP_1:
            addEvent(0,E_MOTOR_PWM_ON,param(MOTOR_SPEED));
            addConditionalEvent(param(SKIP_DELAY),getSensorCondition((Side)arg),E_ACTIVATE_SEQUENCE,S_NEXT_TAPE,arg);
            addConditionalEvent(param(SKIP_DELAY),getSensorCondition((Side)arg),E_PROCESS_EVENTS,D_SKIP_1);
            break;
        case S_NEXT_TAPE_SKIP_2:
            addEvent(0,E_MOTOR_PWM_ON,param(MOTOR_SPEED));
            addConditionalEvent(param(SKIP_DELAY),getSensorCondition((Side)arg),E_ACTIVATE_SEQUENCE,S_NEXT_TAPE_SKIP_1,arg);
            addConditionalEvent(param(SKIP_DELAY),getSensorCondition((Side)arg),E_PROCESS_EVENTS,D_SKIP_2);
            break;
        case S_ENDURANCE_TEST:
            addEvent(0,E_MOTOR_PWM_ON,param(MOTOR_SPEED));
            addEvent(0,E_ACTIVATE_SEQUENCE,S_NEXT_TAPE,SIDE_LEFT);
            addConditionalEvent(0,D_STOP,E_SWITCH_MOTOR_DIRECTION);
            addConditionalEvent(1,D_STOP,E_ACTIVATE_SEQUENCE,S_ENDURANCE_TEST);
            break;
        case S_GRAB_FOOD:
        #if ROBOT == 1
            if(arg == SIDE_LEFT)
            {
                moveArmTimed(0,ARM_DEFAULT,ARM_MIN,p(ARM_MOVE_TIME));
                // addEvent(0,E_MOVE_ARM_TIMED,ARM_MIN,param(ARM_MOVE_TIME));
                addEvent(0,E_SET_PUSHER_LEFT_POS,param(PUSHER_LEFT_DEFAULT_STATE));
                addEvent(2000,E_SET_PUSHER_LEFT_POS,param(RIGHT_GRAB_PUSHER_POS));
                moveArmTimed(3000,ARM_MIN,p(RIGHT_GRAB_ARM_POS),p(ARM_MOVE_TIME));
                // addEvent(3000,E_MOVE_ARM_TIMED,param(RIGHT_GRAB_ARM_POS),param(ARM_MOVE_TIME));
                moveArmTimed(6000,p(RIGHT_GRAB_ARM_POS),ARM_DEFAULT,pARM);
                // addEvent(6000,E_MOVE_ARM_TIMED,ARM_DEFAULT,param(ARM_MOVE_TIME));
                addEvent(6000,E_SET_PUSHER_LEFT_POS,param(PUSHER_LEFT_DEFAULT_STATE));
                _END_SEQ_T(8000,D_GRAB_FOOD);
            }
            else
            {
                moveArmTimed(0,ARM_DEFAULT,ARM_MAX,p(ARM_MOVE_TIME));
                // addEvent(0,E_MOVE_ARM_TIMED,ARM_MAX,param(ARM_MOVE_TIME));
                addEvent(0,E_SET_PUSHER_RIGHT_POS,param(PUSHER_RIGHT_DEFAULT_STATE));
                addEvent(2000,E_SET_PUSHER_RIGHT_POS,param(LEFT_GRAB_PUSHER_POS));
                moveArmTimed(3000,ARM_MAX,p(LEFT_GRAB_ARM_POS),p(ARM_MOVE_TIME));
                // addEvent(3000,E_MOVE_ARM_TIMED,param(LEFT_GRAB_ARM_POS),param(ARM_MOVE_TIME));
                moveArmTimed(6000,p(LEFT_GRAB_ARM_POS),ARM_DEFAULT,pARM);
                // addEvent(6000,E_SET_ARM_POS,ARM_DEFAULT,param(ARM_MOVE_TIME));
                addEvent(6000,E_SET_PUSHER_RIGHT_POS,param(PUSHER_RIGHT_DEFAULT_STATE));
                _END_SEQ_T(7000,D_GRAB_FOOD);
            }
            #elif ROBOT == 2
            moveArmTimed(0,ARM_DEFAULT,ARM_MIN,pARM);
            // addEvent(0,E_MOVE_ARM_TIMED,ARM_MIN,param(ARM_MOVE_TIME));
            addEvent(0,E_SET_PUSHER_LEFT_POS,p(PUSHER_LEFT_DEFAULT_STATE));
            addEvent(2000,E_SET_PUSHER_LEFT_POS,p(LEFT_GRAB_PUSHER_POS));
            moveArmTimed(3000,ARM_MIN,p(LEFT_GRAB_ARM_POS),p(ARM_MOVE_TIME));
            // addEvent(3000,E_MOVE_ARM_TIMED,p(LEFT_GRAB_ARM_POS),pARM);
            moveArmTimed(6000,p(LEFT_GRAB_ARM_POS),ARM_DEFAULT,pARM);
            // addEvent(6000,E_MOVE_ARM_TIMED,ARM_DEFAULT,pARM);
            addEvent(6000,E_SET_PUSHER_LEFT_POS,p(PUSHER_LEFT_DEFAULT_STATE));
            _END_SEQ_T(8000,D_GRAB_FOOD);
            #endif
            break;
        case S_GRAB_CONTINUOUS:
            addEvent(0,E_ACTIVATE_SEQUENCE,S_NEXT_TAPE,param(SENSOR_SIDE));
            addConditionalEvent(0,D_STOP,E_ACTIVATE_SEQUENCE,S_GRAB_FOOD);
            addConditionalEvent(0,D_GRAB_FOOD,E_SWITCH_MOTOR_DIRECTION);
            addConditionalEvent(0,D_GRAB_FOOD,E_ACTIVATE_SEQUENCE,S_GRAB_CONTINUOUS);
            break;
        case S_MOVE_1_GRAB:
            addEvent(0,E_ACTIVATE_SEQUENCE,S_NEXT_TAPE,arg);
            addConditionalEvent(0,D_STOP,E_ACTIVATE_SEQUENCE,S_GRAB_FOOD,arg);
            _END_SEQ_C(D_GRAB_FOOD,D_MOVE_1_GRAB);
            break;
        case S_MOVE_2_GRAB:
            addEvent(0,E_ACTIVATE_SEQUENCE,S_NEXT_TAPE_SKIP_1,arg);
            addConditionalEvent(0,D_STOP,E_ACTIVATE_SEQUENCE,S_GRAB_FOOD,arg);
            _END_SEQ_C(D_GRAB_FOOD,D_MOVE_2_GRAB);
            break;
        case S_PLACE_FOOD:
            moveArmTimed(0,ARM_DEFAULT,ARM_MIN,pARM);
            _EVT_A(E_SET_PUSHER_RIGHT_POS,p(PUSHER_RIGHT_DEFAULT_STATE));
            // _EVT_AA(E_MOVE_ARM_TIMED,60,param(ARM_MOVE_TIME));
            _EVT_TA(3000,E_SET_PUSHER_RIGHT_POS,p(PLACE_FOOD_PUSHER_POS));
            moveArmTimed(5000,ARM_MIN,p(PLACE_FOOD_ARM_POS),pARM);
            // _EVT_TAA(5000,E_MOVE_ARM_TIMED,125,param(ARM_MOVE_TIME));
            moveArmTimed(8000,p(PLACE_FOOD_ARM_POS),ARM_DEFAULT,pARM);
            _EVT_TA(9000,E_SET_PUSHER_RIGHT_POS,p(PUSHER_RIGHT_DEFAULT_STATE));
            // _EVT_TAA(10000,E_MOVE_ARM_TIMED,90,param(ARM_MOVE_TIME));

            _END_SEQ_T(11000,D_PLACE_FOOD);
            break;
        case S_GRAB_PLATE:
            moveArmTimed(0,ARM_DEFAULT,ARM_MIN,pARM);
            addEvent(0,E_SET_PUSHER_LEFT_POS,param(PUSHER_LEFT_DEFAULT_STATE));

            addEvent(2000,E_SET_PUSHER_LEFT_POS,param(PLATE_GRAB_PUSHER_POS));

            moveArmTimed(3000,ARM_MIN,p(PLATE_GRAB_ARM_POS_1),pARM);

            addEvent(6000,E_SET_PUSHER_LEFT_POS,param(PUSHER_LEFT_DEFAULT_STATE));

            moveArmTimed(7000,p(PLATE_GRAB_ARM_POS_1),p(PLATE_GRAB_ARM_POS_1) + p(PLATE_GRAB_DELTA),pARM);

            addEvent(8000,E_SET_PUSHER_LEFT_POS,p(PLATE_GRAB_PUSHER_POS));

            moveArmTimed(9000,p(PLATE_GRAB_ARM_POS_1) + p(PLATE_GRAB_DELTA),p(PLATE_GRAB_ARM_POS_2),pARM);
            
            addEvent(11000,E_SET_PUSHER_LEFT_POS,p(PUSHER_LEFT_DEFAULT_STATE));

            moveArmTimed(12000,p(PLATE_GRAB_ARM_POS_2),ARM_DEFAULT,pARM);

            _END_SEQ_T(13000,D_GRAB_PLATE);
            break;
        case S_GOTO_SERVING_AREA:
            _EVT_A(E_MOTOR_PWM_ON,param(MOTOR_SPEED_FAST) * -1);
            _EVT(E_START_TIMER);
            _EVT_C(C_LEFT_TAPE_SENSOR,E_STOP_TIMER);
            _SEQ_CA(C_LEFT_TAPE_SENSOR,S_TIMED_STOP,SIDE_LEFT);

            _END_SEQ_T(6000,D_GOTO_SERVING);
            break;
        case S_SERVE_BURGER:
            moveArmTimed(0,ARM_DEFAULT,ARM_MIN,p(ARM_MOVE_TIME));
            // _EVT_AA(E_MOVE_ARM_TIMED,ARM_MIN,param(ARM_MOVE_TIME));
            _EVT_A(E_SET_PUSHER_RIGHT_POS,p(PUSHER_RIGHT_DEFAULT_STATE));


            _EVT_TA(4000,E_SET_FLIPPER_POS,180);
            _EVT_TA(7000,E_SET_FLIPPER_POS,0);

            _EVT_TA(10000,E_SET_PUSHER_RIGHT_POS,p(BURGER_SERVE_PUSHER_POS));

            _EVT_TAA(12000,E_MOVE_ARM_TIMED,p(BURGER_SERVE_ARM_POS),p(ARM_MOVE_TIME));
            moveArmTimed(12000,ARM_MIN,p(BURGER_SERVE_ARM_POS),p(ARM_MOVE_TIME));

            moveArmTimed(14000,p(BURGER_SERVE_ARM_POS),ARM_MAX,p(BURGER_SERVING_TIME));

            // _EVT_TAA(20000 + p(BURGER_SERVING_TIME),E_MOVE_ARM_TIMED,90,p(ARM_MOVE_TIME));
            moveArmTimed(18000+p(BURGER_SERVING_TIME),ARM_MAX,ARM_DEFAULT,p(ARM_MOVE_TIME));
            _EVT_TA(19000 + p(BURGER_SERVING_TIME),E_SET_PUSHER_RIGHT_POS,p(PUSHER_RIGHT_DEFAULT_STATE));

            _END_SEQ_T(20000 + p(BURGER_SERVING_TIME),D_SERVE_BURGER);
            break;
        case S_TIMED_STOP:
            _EVT_T(g.timer,E_MOTOR_PWM_OFF);
            break;
            
        #if ROBOT == 1
        case S_HANDOFF_DELTA:
            _EVT_A(E_MOTOR_PWM_ON,param(MOTOR_SPEED_FAST) * sign(param(HANDOFF_DELTA)) * arg);
            _EVT_T(abs(param(HANDOFF_DELTA)),E_MOTOR_PWM_OFF);
            _END_SEQ_T(abs(param(HANDOFF_DELTA)),D_HANDOFF_DELTA);
            break;
        #endif


        #if ROBOT == 1

        //NORMAL ROUTINE

        //START WITH LEFT SENSOR IN FRONT OF TAPE
        case S_STAGE_0:
            g.stage = 0;
            _SPEED(p(MOTOR_SPEED_FAST));
            _SEQ_TA(1,S_NEXT_TAPE,SIDE_LEFT); //go to cutting

            _SEQ_CA(D_STOP,S_HANDOFF_DELTA,1); //handoff delta

            _EVT_CA(D_HANDOFF_DELTA,E_SEND_BT,BT_HANDOFF_READY); //ready for handoff

            _SEQ_CA(C_HANDOFF_READY,S_GRAB_FOOD,SIDE_LEFT); //handoff confirmed, grab top bun
            // _SEQ_CA(D_HANDOFF_DELTA,S_GRAB_FOOD,SIDE_LEFT); //TEMP

            _EVT_C(D_GRAB_FOOD,E_HANDOFF_DONE); //handoff done
            
            _SEQ_CT(1,D_GRAB_FOOD,S_STAGE_1);
            break;
        case S_STAGE_1:
            g.stage = 1;
            _SWITCH;
            _SEQ_TA(1,S_MOVE_1_GRAB,SIDE_RIGHT); //grab cheese

            _SEQ_CT(2,D_MOVE_1_GRAB,S_STAGE_2);
            break;
        case S_STAGE_2:
            g.stage = 2;
            _SEQ_A(S_GRAB_FOOD,SIDE_LEFT); //grab tomato
        
            _SEQ_CT(1,D_GRAB_FOOD,S_STAGE_3);
            break;
        case S_STAGE_3:
            g.stage = 3;
            _SWITCH;
            _SEQ_TA(1,S_NEXT_TAPE_SKIP_1,SIDE_LEFT); //go to cooktop

            _SEQ_CA(D_STOP,S_HANDOFF_DELTA,1); //handoff delta

            _EVT_CA(D_HANDOFF_DELTA,E_SEND_BT,BT_HANDOFF_READY); //ready for handoff

            // _SEQ_CT(2,D_HANDOFF_DELTA,S_STAGE_4); //TEMP
            _SEQ_CT(2,C_HANDOFF_READY,S_STAGE_4); //wait for handoff signal
            break;
        case S_STAGE_4:
            g.stage = 4;
            _EVT_T(1,E_HANDOFF_DONE); //handoff confirmed

            _SEQ_TA(10000,S_GRAB_FOOD,SIDE_LEFT); //grab patty after cooking time

            _SEQ_CT(10001,D_GRAB_FOOD,S_STAGE_5);
            break;
        case S_STAGE_5:
            g.stage = 5;
            _SEQ_A(S_NEXT_TAPE,SIDE_LEFT); //go to plates
            _SEQ_CA(D_STOP,S_GRAB_FOOD,SIDE_RIGHT); //grab lettuce

            _SEQ_CT(1,D_GRAB_FOOD,S_STAGE_6);
            break;
        case S_STAGE_6:
            g.stage = 6;
            _SWITCH;
            _SEQ_TA(1,S_NEXT_TAPE_SKIP_1,SIDE_LEFT); //go to cutting

            _SEQ_CA(D_STOP,S_HANDOFF_DELTA,-1); //handoff delta

            _EVT_CA(D_HANDOFF_DELTA,E_SEND_BT,BT_HANDOFF_READY); //ready for handoff

            _SEQ_CT(1,D_HANDOFF_DELTA,S_STAGE_7);
            break;
        case S_STAGE_7:
            g.stage = 7;
            _SEQ_CA(C_HANDOFF_READY,S_GRAB_FOOD,SIDE_LEFT); //pick up bot bun
            // _SEQ_A(S_GRAB_FOOD,SIDE_LEFT); //TEMP

            _EVT_C(D_GRAB_FOOD,E_HANDOFF_DONE); //handoff done

            _SEQ_CT(1,D_GRAB_FOOD,S_STAGE_8);
            break;
        case S_STAGE_8:
            g.stage = 8;
            _SWITCH;
            _SEQ_TA(1,S_NEXT_TAPE_SKIP_1,SIDE_LEFT); //go to plates
            _SEQ_CT(2,D_STOP,S_GRAB_PLATE); //grab plate

            _SEQ_CT(1,D_GRAB_PLATE,S_STAGE_9);
            break;
        case S_STAGE_9:
            g.stage = 9;
            _SWITCH;
            _SEQ(S_GOTO_SERVING_AREA); //go to serving area
            _SEQ_C(D_GOTO_SERVING,S_SERVE_BURGER); //serve burger
            break;

        
        //CHEESE PLATE ROUTINE
        case S_CHEESE_PLATE_STAGE_0:
            _SPEED(p(MOTOR_SPEED_FAST));
            _SEQ_A(S_GRAB_FOOD,SIDE_RIGHT); //grab cheese

            _SEQ_CT(1,D_GRAB_FOOD,S_CHEESE_PLATE_STAGE_1);
            break;
        case S_CHEESE_PLATE_STAGE_1:
            _SEQ_A(S_NEXT_TAPE_SKIP_2,SIDE_LEFT); //go to plates

            _SEQ_C(D_STOP,S_GRAB_PLATE); //grab plate

            _SEQ_CT(1,D_GRAB_PLATE,S_CHEESE_PLATE_STAGE_2);
            break;
        case S_CHEESE_PLATE_STAGE_2:
            _SWITCH;
            _SEQ(S_GOTO_SERVING_AREA); //go to serving area
            _SEQ_C(D_GOTO_SERVING,S_SERVE_BURGER); //serve cheese plate
            _SEQ_CT(1,D_SERVE_BURGER,S_CHEESE_PLATE_STAGE_3);
            break;
        case S_CHEESE_PLATE_STAGE_3:
            _SEQ_A(S_NEXT_TAPE,SIDE_RIGHT);
            _SWITCH_C(D_STOP);
            break;

        //SALAD PLATE ROUTINE
        case S_SALAD_PLATE_STAGE_0:
            _SPEED(p(MOTOR_SPEED_FAST));
            _SEQ_A(S_GRAB_FOOD,SIDE_LEFT); //grab tomato

            _SEQ_CT(1,D_GRAB_FOOD,S_SALAD_PLATE_STAGE_1);
            break;
        case S_SALAD_PLATE_STAGE_1:
            _SEQ_A(S_NEXT_TAPE_SKIP_2,SIDE_LEFT); //go to plates

            _SEQ_CA(D_STOP,S_GRAB_FOOD,SIDE_RIGHT); //grab lettuce

            _SEQ_CT(1,D_GRAB_FOOD,S_SALAD_PLATE_STAGE_2);
            break;
        case S_SALAD_PLATE_STAGE_2:
            _SEQ(S_GRAB_PLATE); //grab plate

            _SEQ_CT(1,D_GRAB_PLATE,S_SALAD_PLATE_STAGE_3);
            break;
        case S_SALAD_PLATE_STAGE_3:
            _SWITCH;
            _SEQ(S_GOTO_SERVING_AREA); //go to serving area
            _SEQ_C(D_GOTO_SERVING,S_SERVE_BURGER); //serve salad plate

            _SEQ_CT(1,D_SERVE_BURGER,S_SALAD_PLATE_STAGE_4);
            break;
        case S_SALAD_PLATE_STAGE_4:
            _SEQ_A(S_NEXT_TAPE,SIDE_RIGHT);
            _SWITCH_C(D_STOP);
            // _SEQ_CT(1,D_NEXT_TAPE,S_SALAD_PLATE_STAGE_0);
            break;

        #elif ROBOT == 2
        //START WITH LEFT SENSOR BEHIND TAPE
        case S_STAGE_0:
            g.stage = 0;
            _SPEED(p(MOTOR_SPEED_FAST));
            _SEQ_TA(1,S_MOVE_2_GRAB,SIDE_LEFT); //grab top bun

            _SEQ_CT(2,D_MOVE_2_GRAB,S_STAGE_1);
            break;
        case S_STAGE_1:
            g.stage = 1;
            _SWITCH;
            _SEQ_TA(1,S_NEXT_TAPE,SIDE_RIGHT); //go to cutting

            _SEQ_CT(3,D_STOP,S_STAGE_2); 
            break;
        case S_STAGE_2:
            g.stage = 2;
            _SEQ(S_PLACE_FOOD); //handoff

            _EVT_CA(D_PLACE_FOOD,E_SEND_BT,BT_HANDOFF_READY); //handoff signal
            
            _SEQ_CT(3,D_PLACE_FOOD,S_STAGE_3);
            break;
        case S_STAGE_3:
            g.stage = 3;
            _SEQ_A(S_MOVE_1_GRAB,SIDE_LEFT); //grab patty

            _SEQ_CT(1,D_MOVE_1_GRAB,S_STAGE_4);
            break;
        case S_STAGE_4:
            g.stage = 4;
            _SWITCH;
            _SEQ_TA(1,S_NEXT_TAPE_SKIP_1,SIDE_RIGHT); //go to cooktop

            _SEQ_CT(2,D_STOP,S_STAGE_5);
            break;
        case S_STAGE_5:
            g.stage = 5;
            _SEQ(S_PLACE_FOOD); //put patty on cooktop

            _EVT_CA(D_PLACE_FOOD,E_SEND_BT,BT_HANDOFF_READY); //send handoff signal

            _SWITCH;
            _SEQ_CA(D_PLACE_FOOD,S_MOVE_1_GRAB,SIDE_LEFT); //grab bot bun

            _SEQ_CT(2,D_MOVE_1_GRAB,S_STAGE_6);
            break;
        case S_STAGE_6:
            g.stage = 6;
            _SEQ_A(S_NEXT_TAPE,SIDE_RIGHT); //go to cutting

            _SEQ_CT(1,D_STOP,S_STAGE_7);
            break;
        case S_STAGE_7:
            g.stage = 7;
            _SEQ(S_PLACE_FOOD); //handoff

            _EVT_CA(D_PLACE_FOOD,E_SEND_BT,BT_HANDOFF_READY); //handoff signal

            // _SEQ_CT(1,C_NONE,S_STAGE_8); //DONE
            break;
        case S_STAGE_8:
            g.stage = 8;

            _SEQ_CT(1,C_NONE,S_STAGE_9);
            break;
        case S_STAGE_9:
            g.stage = 9;

            break;


        #endif
        
    }
}

void activateSequence(Sequence s)
{
    activateSequence(s,0);
}


void activateEvent(EventName name, int arg1, int arg2)
{
    switch(name)
    {
        case E_LED_ON:
            g.enableLed = true;
            break;
        case E_LED_OFF:
            g.enableLed = false;
            break;
        case E_MOTOR_PWM_ON:
            if(!g.motorOn)
            {
                g.motorOn = true;
                enable_pwm(DRIVE_FWD_CHANNEL,DRIVE_REV_CHANNEL,arg1);
            }
            break;
        case E_MOTOR_PWM_OFF:
            g.motorOn = false;
            enable_pwm(DRIVE_FWD_CHANNEL,DRIVE_REV_CHANNEL,0);
            break;
        case E_SWITCH_MOTOR_DIRECTION:
            setParam(MOTOR_SPEED,-1*param(MOTOR_SPEED));
            break;
        case E_SET_FLIPPER_POS:
            set_servo(FLIPPER_CHANNEL,arg1);
            break;
        case E_SET_ARM_POS:
            set_servo(ARM_CHANNEL,arg1);
            // g.armPos = arg1;
            break;
        case E_MOVE_ARM_TIMED:
            set_servo(ARM_CHANNEL,arg1);
            // moveArmTimed(0,g.armPos,arg1,arg2);
            // g.armPos = arg1;
            break;
        case E_SET_PUSHER_RIGHT_POS:
            set_servo(PUSHER_RIGHT_CHANNEL,arg1);
            break;
        case E_SET_PUSHER_LEFT_POS:
            set_servo(PUSHER_LEFT_CHANNEL,arg1);
            break;
        case E_DISABLE_PUSHER_1:
            ledcWrite(PUSHER_RIGHT_CHANNEL,0);
            break;
        case E_DISABLE_PUSHER_2:
            ledcWrite(PUSHER_LEFT_CHANNEL,0);
            break;
        case E_DISABLE_ARM:
            ledcWrite(ARM_CHANNEL,0);
            break;
        case E_ACTIVATE_SEQUENCE:
            activateSequence((Sequence)arg1,arg2);
            break;
        case E_PROCESS_EVENTS:
            processEvents((Condition)arg1);
            break;
        case E_SEND_BT:
            BTSerial.println((char)arg1);
            break;
        case E_HANDOFF_DONE:
            g.handoffReady = false;
            break;
        case E_SET_MOTOR_SPEED:
            setParam(MOTOR_SPEED,arg1);
            break;
        case E_START_TIMER:
            g.timerStart = millis();
            break;
        case E_STOP_TIMER:
            g.timer = millis() - g.timerStart;
            break;
    }
}

void activateEvent(EventName name, int arg1)
{
    activateEvent(name,arg1,0);
}


void triggerMenuAction(Menu m)
{
    switch(m)
    {
    case SM_MAIN:
        g.currSubMenu = SM_MAIN;
        g.currMenuIndex = g.mainMenuIndex;
        break;
    case SM_DRIVE:
        g.currSubMenu = SM_DRIVE;
        g.currMenuIndex = 0;
        break;
    case SM_ARM:
        g.currSubMenu = SM_ARM;
        g.currMenuIndex = 0;
        break;
    case SM_PUSHERS:
        g.currSubMenu = SM_PUSHERS;
        g.currMenuIndex = 0;
        break;
    case SM_SENSORS:
        g.currSubMenu = SM_SENSORS;
        g.currMenuIndex = 0;
        break;
    case SM_GRAB:
        g.currSubMenu = SM_GRAB;
        g.currMenuIndex = 0;
        break;
    case SM_BT:
        g.currSubMenu = SM_BT;
        g.currMenuIndex = 0;
        break;
    case SM_SEQUENCES:
        g.currSubMenu = SM_SEQUENCES;
        g.currMenuIndex = 0;
        break;
    case SM_ESP:
        g.currSubMenu = SM_ESP;
        g.currMenuIndex = 0;
        break;

    case LED_TEST:
        g.enableLed = !g.enableLed;
        break;
    case EVENT_TEST:
        addEvent(0, E_LED_ON);
        addEvent(1000, E_LED_OFF);
        addEvent(1500, E_LED_ON);
        addEvent(2500, E_LED_OFF);
        break;
    case SWITCH_MOTOR_DIR:
        addEvent(0,E_SWITCH_MOTOR_DIRECTION);
        break;
    case MOTOR_TEST_ACTIVATE:
        addEvent(1000,E_MOTOR_PWM_ON,param(MOTOR_SPEED_FAST));
        addEvent(1000+param(MOTOR_TEST_TIME) * 1000,E_MOTOR_PWM_OFF);

        addEvent(1500+1000*param(MOTOR_TEST_TIME),E_MOTOR_PWM_ON,param(MOTOR_SPEED_FAST) * -1);
        addEvent(1500+2000*param(MOTOR_TEST_TIME),E_MOTOR_PWM_OFF);

        break;
    case MOTOR_TEST_FULL:
        g.fullMotorOn = !g.fullMotorOn;
        if(g.fullMotorOn)
        {
            addEvent(0,E_MOTOR_PWM_ON,param(MOTOR_SPEED_FAST));
            addEvent(0,E_LED_ON);
        }
        else
        {
            addEvent(0,E_MOTOR_PWM_OFF);
            addEvent(0,E_LED_OFF);
        }
        break;
    case CLEAR_PREF:
        pref.clear();
        break;
    case COND_TEST:
        addConditionalEvent(5000,C_LEFT_BUTTON,E_LED_ON);
        addConditionalEvent(5000,C_RIGHT_BUTTON,E_LED_OFF);
        break;
    case TAPE_DRIVE_TEST:
        addEvent(0,E_MOTOR_PWM_ON,param(MOTOR_SPEED));
        addConditionalEvent(50,C_LEFT_TAPE_SENSOR,E_ACTIVATE_SEQUENCE,S_STOP_MOTOR);
        break;
    case SET_FLIPPER_POS:
        addEvent(0,E_SET_FLIPPER_POS,param(FLIPPER_POS));
        break;
    case SET_ARM_POS:
        addEvent(1000,E_SET_ARM_POS,param(ARM_POS));
        break;
    case SET_PUSHER_1_POS:
        addEvent(0,E_SET_PUSHER_RIGHT_POS,param(PUSHER_1_POS));
        break;
    case SET_PUSHER_2_POS:
        addEvent(0,E_SET_PUSHER_LEFT_POS,param(PUSHER_2_POS));
        break;
    case DISABLE_PUSHER_1:
        addEvent(0,E_DISABLE_PUSHER_1);
        break;
    case DISABLE_PUSHER_2:
        addEvent(0,E_DISABLE_PUSHER_2);
        break;
    case DISABLE_ARM:
        addEvent(0,E_DISABLE_ARM);
        break;
    case NEXT_TAPE_TEST:
        addEvent(2000,E_ACTIVATE_SEQUENCE,S_NEXT_TAPE,param(SENSOR_SIDE));
        // addConditionalEvent(1999,C_NEXT_TAPE_DONE,E_ACTIVATE_SEQUENCE,S_STOP_MOTOR);
        break;
    case SKIP_1_TEST:
        addEvent(2000,E_ACTIVATE_SEQUENCE,S_NEXT_TAPE_SKIP_1,param(SENSOR_SIDE));
        // addConditionalEvent(1999,D_STOP,E_ACTIVATE_SEQUENCE,S_GRAB_FOOD);
        // addConditionalEvent(1999,C_SKIP_1_DONE,E_ACTIVATE_SEQUENCE,S_STOP_MOTOR);
        break;
    case SKIP_2_TEST:
        addEvent(2000,E_ACTIVATE_SEQUENCE,S_NEXT_TAPE_SKIP_2,param(SENSOR_SIDE));
        // addConditionalEvent(1999,C_SKIP_2_DONE,E_ACTIVATE_SEQUENCE,S_STOP_MOTOR);
        break;
    case ENDURANCE_TEST:
        addEvent(2000,E_ACTIVATE_SEQUENCE,S_ENDURANCE_TEST);
        break;
    case ARM_TIMED:
        // addEvent(1000,E_MOVE_ARM_TIMED,param(ARM_POS),param(ARM_MOVE_TIME));
        moveArmTimed(1000,90,param(ARM_POS),param(ARM_MOVE_TIME));
        break;
    case BT_CONNECT:
        #if ROBOT == 1
        connectBT();
        #endif
        BTSerial.println(BT_LED);
        break;
    case GRAB_FOOD:
        addEvent(3000,E_ACTIVATE_SEQUENCE,S_GRAB_FOOD,param(SENSOR_SIDE));
        break;
    case STAGES:
        addEvent(3000,E_ACTIVATE_SEQUENCE,S_STAGE_0);
        break;
    case GOTO_SERVING_AREA:
        _SEQ_T(3000,S_GOTO_SERVING_AREA);
        break;
    case SERVE_BURGER:
        _SEQ_T(3000,S_SERVE_BURGER);
        break;
    case SEND_HANDOFF_SIGNAL:
        _EVT_A(E_SEND_BT,BT_HANDOFF_READY);
        break;
    case CLEAR_EVENTS:
        _EVT(E_CLEAR_EVENTS);
        break;
    case SLOW_DRIVE_TEST:
        _SPEED(p(MOTOR_SPEED_FAST));
        _SEQ_A(S_NEXT_TAPE_SKIP_1,p(SENSOR_SIDE));
        _SPEED_C(D_SKIP_1,p(MOTOR_SPEED_SLOW));
        break;

    #if ROBOT == 1
    case GRAB_PLATE:
        _SEQ_T(3000,S_GRAB_PLATE);
        break;
    case CHEESE_PLATE:
        _SEQ_T(3000,S_CHEESE_PLATE_STAGE_0);
        break;
    case SALAD_PLATE:
        _SEQ_T(3000,S_SALAD_PLATE_STAGE_0);
        break;
    #elif ROBOT == 2
    case PLACE_FOOD:
        _SEQ_T(3000,S_PLACE_FOOD);
        break;
    #elif ROBOT == 3
    case ACTIVATE_DEBUG_PIN:
        pinMode(param(DEBUG_PIN),OUTPUT);
        g.debugPinOn = !g.debugPinOn;
        digitalWrite(param(DEBUG_PIN),g.debugPinOn);
        break;
    #endif

    }
}



//CUSTOM FUNCTIONS

//degrees are 0 to 180
// duty cycle of 1.75% - 11.75%
void set_servo(int channel, int degrees)
{
    ledcWrite(channel, (int)((0.1f * degrees / 180 + 0.0175f) * SERVO_PWM_RESOLUTION));
}

void enable_pwm(PWMChannel fwd_channel,PWMChannel rev_channel,int pwmVal)
{
    if(pwmVal > 0)
    {
        ledcWrite(fwd_channel,pwmVal);
        ledcWrite(rev_channel,0);
    }
    else if(pwmVal < 0)
    {
        ledcWrite(fwd_channel,0);
        ledcWrite(rev_channel,-pwmVal);
    }
    else
    {
        ledcWrite(fwd_channel,0);
        ledcWrite(rev_channel,0);
    }
}



//MENU FUNCTIONALITY


void handlePress(int b, bool repeat)
{
    switch(b)
    {
        case LEFT_BUTTON:
            leftButton();
            processEvents(C_LEFT_BUTTON);
            break;
        case SELECT_BUTTON:
            selectButton();
            processEvents(C_SELECT_BUTTON);
            break;
        case RIGHT_BUTTON:
            rightButton();
            processEvents(C_RIGHT_BUTTON);
            break;
    }
}


void selectButton()
{
    if(currMenuItem().isVariable)
    {
        g.inMenu = !g.inMenu;
    }
    else
    {
        triggerMenuAction(currMenu());
    }
}

void leftButton()
{
    if(g.inMenu)
    {
        changeParam(currMenu(), -1 * currMenuItem().delta);
    }
    else
    {
        prevMenu();
    }
}

void rightButton()
{
    if(g.inMenu)
    {
        changeParam(currMenu(), currMenuItem().delta);
    }
    else
    {
        nextMenu();
    }
}