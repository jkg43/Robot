#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Preferences.h>
#include "driver/ledc.h"
#include <unordered_map>
#include <list>
#include <vector>
#include "globals.h"
#include "types.h"
#include "tools.h"
#include "menus.h"
#include "actions.h"
#include "event.h"
#include "sensors.h"
#include "defns.h"
#include "bluetooth.h"

using namespace std;


//Global objects
Globals g;
Preferences pref;
list<Event> events;
Adafruit_SSD1306 display_handler(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
extern unordered_map<Menu,MenuItem> menus;
extern unordered_map<Menu,vector<Menu>> subMenus;
extern BluetoothSerial BTSerial;
//SDA Pin 21, SCK Pin 22












void setup()
{

    //SERIAL SETUP
    Serial.begin(115200);
    Serial.println("Initializing");


    //PIN SETUP
    pinMode(LED_PIN,OUTPUT);
    for (int i = 0; i < NUM_BUTTONS; i++)
    {
        pinMode(buttonPins[i],INPUT_PULLUP);
    }
    pinMode(LEFT_TAPE_SENSOR_PIN,INPUT);
    pinMode(RIGHT_TAPE_SENSOR_PIN,INPUT);
    pinMode(THRESHOLD_OUTPUT_PIN,OUTPUT);

    //SENSOR SETUP
    attachInterrupt(LEFT_TAPE_SENSOR_PIN,left_tape_sensor_interrupt,RISING);
    attachInterrupt(RIGHT_TAPE_SENSOR_PIN,right_tape_sensor_interrupt,RISING);
    

    //MOTOR SETUP
    pwmSetup(DRIVE_FWD_CHANNEL,DRIVE_MOTOR_FWD_PIN,MOTOR_PWM_FREQ,MOTOR_PWM_BITS);
    pwmSetup(DRIVE_REV_CHANNEL,DRIVE_MOTOR_REV_PIN,MOTOR_PWM_FREQ,MOTOR_PWM_BITS);


    //SERVO SETUP
    pwmSetup(FLIPPER_CHANNEL,FLIPPER_SERVO_PIN,SERVO_PWM_FREQ,SERVO_PWM_BITS);
    pwmSetup(ARM_CHANNEL,ARM_SERVO_PIN_1,SERVO_PWM_FREQ,SERVO_PWM_BITS);
    pwmSetup(ARM_CHANNEL,ARM_SERVO_PIN_2,SERVO_PWM_FREQ,SERVO_PWM_BITS);
    pwmSetup(PUSHER_RIGHT_CHANNEL,PUSHER_1_PIN,SERVO_PWM_FREQ,SERVO_PWM_BITS);
    pwmSetup(PUSHER_LEFT_CHANNEL,PUSHER_2_PIN,SERVO_PWM_FREQ,SERVO_PWM_BITS);


    //DISPLAY SETUP
    display_handler.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display_handler.clearDisplay();


    //LOAD PREFERENCES
    pref.begin("robot",false);
    for(auto& x : menus)
    {
        MenuItem& m = x.second;
        if(m.isVariable)
        {
            m.val = pref.getInt(m.name.substr(0,15).c_str(),m.val);
        }
    }

    //BT SETUP
    initBT();

}

void loop() {
 
    //LED
    digitalWrite(LED_PIN,g.enableLed);


    //BUTTONS
    for (int i = 0; i < NUM_BUTTONS; i++)
    {
        checkButton(i);
    }

    //SENSORS
    dacWrite(THRESHOLD_OUTPUT_PIN,param(TAPE_SENSOR_THRESHOLD));
    // updateSensors();

    //BT
    updateBT();

    
    //TIMED EVENTS
    processEvents(C_NONE);


    //HANDOFF EVENTS - to make sure they trigger event if
    //  signal was received before events were queued
    if(g.handoffReady)
    {
        processEvents(C_HANDOFF_READY);
    }


    //DISPLAY
    display_handler.clearDisplay();
    display_handler.setTextSize(1);
    display_handler.setTextColor(SSD1306_WHITE);
    display_handler.setCursor(0,0);

    const MenuItem& curr = currMenuItem();


    if(g.inMenu)
    {
        display_handler.printf("%s:\n%d\nR: %d, %d\nD: %d\n",curr.name.c_str(),curr.val,curr.min,curr.max,curr.delta);
    }
    else if(curr.isVariable)
    {
        display_handler.printf("%s:\n%s\nVal: %d\n",currSubMenuName().c_str(),curr.name.c_str(),curr.val);
    }
    else if(g.currSubMenu == SM_MAIN)
    {
        if(currMenu() == CLEAR_EVENTS)
        {
            display_handler.setTextSize(2);
            display_handler.println("\nRESET");
            display_handler.setTextSize(1);
        }
        else
        {
            display_handler.printf("Open Menu:\n%s\n",curr.name.c_str());
        }
    }
    else if(currMenu() == SM_MAIN)
    {
        display_handler.printf("%s:\n\n\n%s\n",currSubMenuName().c_str(),curr.name.c_str());
    }
    else
    {
        display_handler.printf("%s:\nActivate:\n%s\n",currSubMenuName().c_str(),curr.name.c_str());
    }
    customDisplay(currMenu());



    display_handler.display();



}


void customDisplay(Menu p)
{
    switch(p)
    {
        case TAPE_SENSOR_THRESHOLD:
            display_handler.printf("Left: %d\n",digitalRead(LEFT_TAPE_SENSOR_PIN));
            display_handler.printf("Right: %d\n",digitalRead(RIGHT_TAPE_SENSOR_PIN));
            break;
        case SENSOR_SIDE:
            if(param(SENSOR_SIDE) == 1)
            {
                display_handler.println("Right");
            }
            else
            {
                display_handler.println("Left");
            }
            break;
        case BT_CONNECT:

            display_handler.print("Local: ");

            #if ROBOT == 1
            display_handler.print("Robot 1\n");
            #elif ROBOT == 2
            display_handler.print("Robot 2\n");
            #endif


            uint8_t mac[6];

            BTSerial.getBtAddress(mac);

            for (size_t i = 0; i < 5; i++)
            {
                display_handler.printf("%02X:",mac[i]);
            }
            display_handler.printf("%02X\n",mac[5]);

            display_handler.printf("Connected: %d\n",BTSerial.connected());

            break;
        case STAGES:
            display_handler.printf("Stage: %d",g.stage);
            break;

        #if ROBOT == 3
        case ACTIVATE_DEBUG_PIN:
            display_handler.printf("Active: %d",g.debugPinOn);
            break;
        #endif
    }
}




