// Copyright (c) 2020 Hugh Wade
// SPDX-License-Identifier: MIT
#include <FastLED.h>
#define NUM_LEDS 3
#define DATA_PIN 12
#define PWM1_PIN 11
#define PWM2_PIN 10
#define PWM3_PIN 9

#define UPWARDS 1
#define NONE 0
#define DOWNWARDS -1
#define BOTTOM 1
#define TOP 255

 //host commands
#define CMD_CAL 120
#define CMD_DEMO 121
#define CMD_SET_VU1 130
#define CMD_SET_VU2 131
#define CMD_SET_VU3 132
#define CMD_SET_H_LED1 133
#define CMD_SET_S_LED1 134
#define CMD_SET_V_LED1 135
#define CMD_SET_H_LED2 136
#define CMD_SET_S_LED2 137
#define CMD_SET_V_LED2 138
#define CMD_SET_H_LED3 139
#define CMD_SET_S_LED3 140
#define CMD_SET_V_LED3 141

//Misc
#define COMMAND_BUFFER_LENGTH 2

uint8_t commandReadIndex = 0;
uint8_t commandReadBuffer[COMMAND_BUFFER_LENGTH] = {0, 0};
CRGB leds[NUM_LEDS];
CHSV hsvs[NUM_LEDS];

uint8_t runDemo = 1;

void setup(){
    FastLED.addLeds<WS2811, DATA_PIN>(leds, NUM_LEDS);
    for(int i = 0; i < NUM_LEDS; i++){
        leds[i] = CRGB::Green;
        hsvs[i] = CHSV(HSVHue::HUE_GREEN, 255, 255);
    }

    pinMode(PWM1_PIN, OUTPUT);
    pinMode(PWM2_PIN, OUTPUT);
    pinMode(PWM3_PIN, OUTPUT);
    pinMode(DATA_PIN, OUTPUT);
    Serial.begin(115200);
}


void loop(){
    processHostCommands();

    if(runDemo == 1)
        demo(5);
    else
        FastLED.show();    
}

void processHostCommands()
{  
    //the hardware ring buffer is 64bytes
    //if we don't drain it fast enough new data is dropped
    if(!Serial.available())
        return;

    commandReadBuffer[commandReadIndex++] = Serial.read();
    if(commandReadIndex == COMMAND_BUFFER_LENGTH)
    {
        commandReadIndex = 0;
        uint8_t cmd = commandReadBuffer[0];
        uint8_t data = commandReadBuffer[1];
    
        switch(cmd)
        {
            case CMD_CAL:
                if(data == 1){
                    analogWrite(PWM1_PIN, 255);
                    analogWrite(PWM2_PIN, 255);
                    analogWrite(PWM3_PIN, 255);
                }
                else{
                    analogWrite(PWM1_PIN, 0);
                    analogWrite(PWM2_PIN, 0);
                    analogWrite(PWM3_PIN, 0);
                }
                break;
            case CMD_DEMO:
                if(data == 1){
                    runDemo = 1;
                }else{
                    runDemo = 0;
                    analogWrite(PWM1_PIN, 0);
                    analogWrite(PWM2_PIN, 0);
                    analogWrite(PWM3_PIN, 0);
                    for(int i = 0; i < NUM_LEDS; i++){
                        leds[i] = CRGB::Green;
                        hsvs[i] = CHSV(HSVHue::HUE_GREEN, 255, 255);
                    }
                }
                break;
            case CMD_SET_VU1:
                analogWrite(PWM1_PIN, data);
                break;
            case CMD_SET_VU2:
                analogWrite(PWM2_PIN, data);
                break;
            case CMD_SET_VU3:
                analogWrite(PWM3_PIN, data);
                break;
            case CMD_SET_H_LED1:
                hsvs[0].h = data;
                leds[0].setHSV(hsvs[0].h, hsvs[0].s, hsvs[0].v);
                break;
            case CMD_SET_S_LED1:
                hsvs[0].h = data;
                leds[0].setHSV(hsvs[0].h, hsvs[0].s, hsvs[0].v);
                break;
            case CMD_SET_V_LED1:
                hsvs[0].h = data;
                leds[0].setHSV(hsvs[0].h, hsvs[0].s, hsvs[0].v);
                break;
            case CMD_SET_H_LED2:
                hsvs[1].h = data;
                leds[1].setHSV(hsvs[1].h, hsvs[1].s, hsvs[1].v);
                break;
            case CMD_SET_S_LED2:
                hsvs[1].h = data;
                leds[1].setHSV(hsvs[1].h, hsvs[1].s, hsvs[1].v);
                break;
            case CMD_SET_V_LED2:
                hsvs[1].h = data;
                leds[1].setHSV(hsvs[1].h, hsvs[1].s, hsvs[1].v);
                break;
            case CMD_SET_H_LED3:
                hsvs[2].h = data;
                leds[2].setHSV(hsvs[2].h, hsvs[2].s, hsvs[2].v);
                break;
            case CMD_SET_S_LED3:
                hsvs[2].h = data;
                leds[2].setHSV(hsvs[2].h, hsvs[2].s, hsvs[2].v);
                break;
            case CMD_SET_V_LED3:
                hsvs[2].h = data;
                leds[2].setHSV(hsvs[2].h, hsvs[2].s, hsvs[2].v);
                break;
        }
    }
}

void demo(uint16_t delay_ms){
    static uint8_t fade_step = 1;
    static uint8_t fade_direction = UPWARDS;
    FastLED.showColor(CHSV(72, 240, fade_step)); 
    
    if(fade_step == TOP)
        fade_direction = DOWNWARDS;
    else if(fade_step == BOTTOM)
        fade_direction = UPWARDS;
    
    fade_step = fade_step + fade_direction;

    analogWrite(PWM1_PIN, fade_step);
    analogWrite(PWM2_PIN, fade_step);
    analogWrite(PWM3_PIN, fade_step);

    delay(delay_ms);
}
