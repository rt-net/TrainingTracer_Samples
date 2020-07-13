/*
 *    Arduino_Exercises/Exercise11/Exercise11.ino
 *    
 *    Copyright(C) 2020 RT Corporation <support@rt-net.jp>
 *      All rights reserved.
 *      
 *    License: Apache License, Version 2.0
 *     https://www.apache.org/licenses/LICENSE-2.0
 *     
 */

int DIR_R_Pin = D12;
int DIR_L_Pin = D0;
int SW1_Pin = D7;
int SW2_Pin = D8;
int PWM_R_Pin = D11;
int PWM_L_Pin = D10;
int CW_R  = LOW;
int CCW_R = HIGH;
int CW_L = HIGH;
int CCW_L = LOW;

int PWM =0;

void setup() {
  // put your setup code here, to run once:
  pinMode(DIR_R_Pin,OUTPUT);
  pinMode(DIR_L_Pin,OUTPUT);

  pinMode(SW1_Pin,INPUT_PULLUP);
  pinMode(SW2_Pin,INPUT_PULLUP);

  digitalWrite(DIR_R_Pin,CW_R);
  digitalWrite(DIR_L_Pin,CW_L);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(SW1_Pin) == LOW){
    delay(200);
    PWM += 10;
    if(PWM > 255){
      PWM = 255;
    }
    analogWrite(PWM_R_Pin,PWM);
    analogWrite(PWM_L_Pin,PWM);
  }
  if(digitalRead(SW2_Pin) == LOW){
    delay(200);
    PWM -= 10;
    if(PWM < 0){
      PWM = 0;
    }
    analogWrite(PWM_R_Pin,PWM);
    analogWrite(PWM_L_Pin,PWM);
  }
 }
