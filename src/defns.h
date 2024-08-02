#pragma once

#define ROBOT 1



//SCREEN DEFNS
#define SCREEN_WIDTH 128 // OLED display width, in pixels65
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET 	-1 // This display does not have a reset pin accessible

//BUTTON DEFNS
#define NUM_BUTTONS 3
const int buttonPins[3] = {26,32,14};

//SENSOR DEFNS
#define LEFT_TAPE_SENSOR_PIN 38
#define RIGHT_TAPE_SENSOR_PIN 37
#define THRESHOLD_OUTPUT_PIN 25

//LED DEFNS
#define LED_PIN 15

//MOTOR DEFNS
#define MOTOR_PWM_FREQ 100
#define MOTOR_PWM_BITS 12
#define MOTOR_PWM_RESOLUTION 4096
#define DRIVE_MOTOR_FWD_PIN 19
#define DRIVE_MOTOR_REV_PIN 20

//SERVO DEFNS
#define SERVO_PWM_FREQ 50
#define SERVO_PWM_BITS 12
#define SERVO_PWM_RESOLUTION 4096
#define FLIPPER_SERVO_PIN 27
#define ARM_SERVO_PIN_1 2
#define ARM_SERVO_PIN_2 4
#define PUSHER_1_PIN 13
#define PUSHER_2_PIN 12
// #define PUSHER_1_DEFAULT_STATE 90
// #define PUSHER_2_DEFAULT_STATE 90

#if ROBOT == 1
#define ARM_DEFAULT 90
#define ARM_MIN 50 
#define ARM_MAX 130 
#elif ROBOT == 2
#define ARM_DEFAULT 90
#define ARM_MIN 40
#define ARM_MAX 130 
#endif