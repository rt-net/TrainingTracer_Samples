/*
 *    Mbed_Sample/STEP6/main.cpp
 *    
 *    Copyright(C) 2020 RT Corporation <support@rt-net.jp>
 *      All rights reserved.
 *      
 *    License: Apache License, Version 2.0
 *     https://www.apache.org/licenses/LICENSE-2.0
 *     
 */

#include "mbed.h"

DigitalOut DIR_R(D12);
DigitalOut DIR_L(D0);
DigitalIn SW1(D7);
DigitalIn SW2(D8);
PwmOut PWM_R(D11);
PwmOut PWM_L(D10);

int CW_R = 0;
int CCW_R = 1;
int CW_L = 1;
int CCW_L = 0;

float PWM =0;

int main() {
    SW1.mode(PullUp);
    SW2.mode(PullUp);
    
    PWM_R.period(0.001);
    PWM_L.period(0.001);
    
    DIR_R = CW_R;
    DIR_L = CW_L;
    while(1) {
        if(SW1 == 0){
            wait(0.2);
            PWM += 0.1f;
            if(PWM > 1){
                PWM =1;
            }
            PWM_R=PWM;
            PWM_L=PWM;
        }
        if(SW2 == 0 ){
            wait(0.2);
            PWM -= 0.1f;
            if(PWM < 0){
                PWM =0;
            }
            PWM_R=PWM;
            PWM_L=PWM;
        }
    }
}
