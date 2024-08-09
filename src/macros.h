#pragma once





#define _NEXT_L addEvent(0,E_ACTIVATE_SEQUENCE,S_NEXT_TAPE,SIDE_LEFT)
#define _SKIP_1_L addEvent(0,E_ACTIVATE_SEQUENCE,S_NEXT_TAPE_SKIP_1,SIDE_LEFT)
#define _SKIP_2_L addEvent(0,E_ACTIVATE_SEQUENCE,S_NEXT_TAPE_SKIP_2,SIDE_LEFT)

#define _NEXT_R addEvent(0,E_ACTIVATE_SEQUENCE,S_NEXT_TAPE,SIDE_RIGHT)
#define _SKIP_1_R addEvent(0,E_ACTIVATE_SEQUENCE,S_NEXT_TAPE_SKIP_1,SIDE_RIGHT)
#define _SKIP_2_R addEvent(0,E_ACTIVATE_SEQUENCE,S_NEXT_TAPE_SKIP_2,SIDE_RIGHT)

#define _STOP(cond) addConditionalEvent(0,cond,E_ACTIVATE_SEQUENCE,S_STOP_MOTOR)

#define _END_SEQ(delay,condition,done) addConditionalEvent(delay,condition,E_PROCESS_EVENTS,done)
#define _END_SEQ_C(condition,done) addConditionalEvent(0,condition,E_PROCESS_EVENTS,done)
#define _END_SEQ_T(delay,done) addEvent(delay,E_PROCESS_EVENTS,done)

#define _SEQ_CT(delay,cond,seq) addConditionalEvent(delay,cond,E_ACTIVATE_SEQUENCE,seq)
#define _SEQ_C(cond,seq) addConditionalEvent(0,cond,E_ACTIVATE_SEQUENCE,seq)
#define _SEQ_T(delay,seq) addEvent(delay,E_ACTIVATE_SEQUENCE,seq)
#define _SEQ_TA(delay,seq,arg) addEvent(delay,E_ACTIVATE_SEQUENCE,seq,arg)
#define _SEQ(seq) addEvent(0,E_ACTIVATE_SEQUENCE,seq)
#define _SEQ_A(seq,arg) addEvent(0,E_ACTIVATE_SEQUENCE,seq,arg)
#define _SEQ_CA(cond,seq,arg) addConditionalEvent(0,cond,E_ACTIVATE_SEQUENCE,seq,arg)


#define _EVT(evt) addEvent(0,evt)
#define _EVT_T(delay,evt) addEvent(delay,evt)
#define _EVT_A(evt,arg) addEvent(0,evt,arg)
#define _EVT_AA(evt,arg1,arg2) addEvent(0,evt,arg1,arg2)
#define _EVT_C(cond,evt) addConditionalEvent(0,cond,evt)
#define _EVT_CA(cond,evt,arg) addConditionalEvent(0,cond,evt,arg)
#define _EVT_TA(delay,evt,arg) addEvent(delay,evt,arg)
#define _EVT_TAA(delay,evt,arg1,arg2) addEvent(delay,evt,arg1,arg2)
#define _EVT_CT(delay,cond,evt) addConditionalEvent(delay,cond,evt)

#define _SWITCH _EVT(E_SWITCH_MOTOR_DIRECTION)
#define _SWITCH_C(cond) _EVT_C(cond,E_SWITCH_MOTOR_DIRECTION)

#define _SPEED(speed,mult) _EVT_AA(E_SET_MOTOR_SPEED,speed,mult)
#define _SPEED_C(cond,speed,mult) addConditionalEvent(0,cond,E_SET_MOTOR_SPEED,speed,mult)

#define p(par) param(par)
#define pARM param(ARM_MOVE_TIME)
#define pPUSHER param(PUSHER_MOVE_TIME)
#define pFLIP param(FLIP_TIME)
#define pSERVE param(BURGER_SERVING_TIME)
#define pPLATE param(PLATE_GRAB_ARM_TIME)