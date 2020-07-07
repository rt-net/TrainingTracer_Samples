/*
 *    Mbed_Sample/STEP8/main.cpp
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
PwmOut PWM_R(D11);
PwmOut PWM_L(D10);

DigitalIn SW1(D7);
DigitalIn SW2(D8);

AnalogIn LINE_L2(A5);
AnalogIn LINE_L1(A4);
AnalogIn LINE_R1(A3);
AnalogIn LINE_R2(A2);
AnalogIn MarkerR(D3);
AnalogIn MarkerL(D6);

DigitalOut LED(D13);
DigitalOut BUZZER(D2);

int CW_R = 0;
int CCW_R = 1;
int CW_L = 1;
int CCW_L = 0;

float PWM_R_Duty =0;
float PWM_L_Duty =0;


//変数宣言
float Line_Controll;
float inside_offset = -0.095;
float outside_offset = -0.131;

//ラインが白の場合:1、ラインが黒の場合-1
float Line_signed = -1.0f;

int main() {
    
    SW1.mode(PullUp);
    SW2.mode(PullUp);
    
    PWM_R.period(0.001);
    PWM_L.period(0.001);    
    
    LED = 1;
    wait(0.1);
    LED = 0;
    wait(0.1);
    LED = 1;
    wait(0.1);
    LED = 0;
    wait(0.1);
        
    while(1) {
        if(SW2 == 0 ){
            BUZZER = 1;
            wait(0.1);
            BUZZER = 0;
            wait(0.1);
            while(1){
                printf("\n\r LL2=%.3f LL1=%.3f LR1=%.3f LR2=%.3f inside_Offset=%.3f,outside_Offset=%.3f ML=%.3f MR=%.3f",
                LINE_L2.read(),LINE_L1.read(),LINE_R1.read(),LINE_R2.read(),
                (LINE_L2.read() - LINE_L1.read()),(LINE_R1.read() - LINE_R2.read()),
                MarkerL.read(),MarkerR.read()
                );
                wait(0.1);
                if(SW2 == 0 ){
                    wait(0.2);
                    break;
                }
            }
        }
        if(SW1 == 0){
            LED = 1;
            BUZZER = 1;
            wait(0.07);
            BUZZER = 0;
            wait(0.07);
            BUZZER = 1;
            wait(0.07);
            BUZZER = 0;
            wait(0.07);
            break;
        }            
    }
    
    while(1){
//マーカーを検出?
    if(Line_signed > 0.0f ){
        if(MarkerR > 0.05f ){
            LED = 0;
            PWM_L = 0;
            PWM_R = 0;
            BUZZER = 1;
            wait(0.1);
            BUZZER = 0;
            while(1){
                if(SW1 == 0){
                    LED = 1;
                    break;
                }
            }
        }
    }else{
        if(MarkerR < 0.040f ){
            LED = 0;
            PWM_L = 0;
            PWM_R = 0;
            BUZZER = 1;
            wait(0.1);
            BUZZER = 0;
            while(1){
                if(SW1 == 0){
                    LED = 1;
                    break;
                }
            }
        }
    }
            
        
        
//ラインセンサの値から制御量を算出する
        Line_Controll = (LINE_L1.read() - LINE_R1.read() -inside_offset)
                        + 3.0f * (LINE_L2.read() - LINE_R2.read() -outside_offset);
        PWM_L_Duty = 0.3f - Line_signed * Line_Controll/3.0f;
        PWM_R_Duty = 0.3f + Line_signed * Line_Controll/3.0f;
        
        if( PWM_L_Duty < 0){
            DIR_L = CCW_L;
        }else{
            DIR_L = CW_L;
        }
        PWM_L_Duty = abs(PWM_L_Duty);
        if(PWM_L_Duty > 1){
            PWM_L_Duty = 1;
        }
        if(PWM_L_Duty < 0){
            PWM_L_Duty = 0 ;
        }
        PWM_L=PWM_L_Duty;
        printf("\n\r L_duty=%.3f",PWM_L.read());
        
        if(PWM_R_Duty < 0){
            DIR_R = CCW_R;
        }else{
            DIR_R = CW_R;
        }
        PWM_R_Duty = abs(PWM_R_Duty);
        if(PWM_R_Duty > 1){
            PWM_R_Duty = 1;
        }
        if(PWM_R_Duty < 0){
            PWM_R_Duty = 0;
        }
        PWM_R=PWM_R_Duty;
        printf("\n\r R_duty=%.3f",PWM_R.read());
            
        wait(0.001);
    }
}