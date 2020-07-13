/*
 *    Arduino_Exercises/Exercise13/Exercise13.ino
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
int CW_R  = 0;
int CCW_R = 1;
int CW_L = 1;
int CCW_L = 0;

int PWM =50;

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
    digitalWrite(DIR_R_Pin,CW_R);
    digitalWrite(DIR_L_Pin,CW_L);
    analogWrite(PWM_R_Pin,PWM);
    analogWrite(PWM_L_Pin,PWM);    
  }
  if(digitalRead(SW2_Pin) == LOW){
    delay(200);
    digitalWrite(DIR_R_Pin,CCW_R);
    digitalWrite(DIR_L_Pin,CCW_L);
    analogWrite(PWM_R_Pin,PWM);
    analogWrite(PWM_L_Pin,PWM);
    }
 }
