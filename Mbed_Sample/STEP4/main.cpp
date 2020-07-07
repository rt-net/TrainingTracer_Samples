/*
 *    Mbed_Sample/STEP4/main.cpp
 *    
 *    Copyright(C) 2020 RT Corporation <support@rt-net.jp>
 *      All rights reserved.
 *      
 *    License: Apache License, Version 2.0
 *     https://www.apache.org/licenses/LICENSE-2.0
 *     
 */

#include "mbed.h"

DigitalOut myled(D13);
DigitalOut BUZZER(D2);
DigitalIn SW1(D7);
DigitalIn SW2(D8);

int count =0;

int main() {
    SW1.mode(PullUp);
    SW2.mode(PullUp);
    
    while(1) {
        if(SW1 == 0){
            myled = 1;
            count++;
            wait(0.03);
            while(SW1 == 0);
            wait(0.03);
        }else{
            myled = 0;
        }
        
        if(SW2 == 0){
            for(int i =0;i<count;i++){
                BUZZER = 1;
                wait(0.1);
                BUZZER = 0;
                wait(0.5);
            }
            count = 0;
        }        
    }
}