/*
 *    Mbed_Sample/STEP3/main.cpp
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

int main() {
    SW1.mode(PullUp);
    SW2.mode(PullUp);
    
    while(1) {
        if(SW1 == 0){
            myled = 1;
        }else{
            myled = 0;
        }
        if(SW2 == 0){
            BUZZER = 1;
        }else{
            BUZZER = 0;
        }
    }
}
