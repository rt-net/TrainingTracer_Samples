/*
 *    Mbed_Sample/STEP5/main.cpp
 *    
 *    Copyright(C) 2020 RT Corporation <support@rt-net.jp>
 *      All rights reserved.
 *      
 *    License: Apache License, Version 2.0
 *     https://www.apache.org/licenses/LICENSE-2.0
 *     
 */

#include "mbed.h"

AnalogIn LINE_L2(A5);
AnalogIn LINE_L1(A4);
AnalogIn LINE_R1(A3);
AnalogIn LINE_R2(A2);
DigitalOut myled(D13);

int main() {
    while(1) {
        printf("\n\r LL2=%.3f LL1=%.3f LR1=%.3f LR2=%.3f",
        LINE_L2.read(),LINE_L1.read(),LINE_R1.read(),LINE_R2.read());
        myled = 1; // LED is ON
        wait(0.1); // 100 ms
        myled = 0; // LED is OFF
        wait(0.1); // 100 ms
    }
}
