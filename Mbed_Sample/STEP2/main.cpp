/*
 *    Mbed_Sample/STEP2/main.cpp
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

int main() {
    while(1) {
        myled = 1; // LED is ON
        BUZZER =1;
        wait(1.0); // 1 s
        myled = 0; // LED is OFF
        BUZZER = 0;
        wait(1.0); // 1 sec
    }
}