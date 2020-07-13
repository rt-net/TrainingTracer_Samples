/*
 *    Arduino_Exercises/Exercise9/Exercise9.ino
 *    
 *    Copyright(C) 2020 RT Corporation <support@rt-net.jp>
 *      All rights reserved.
 *      
 *    License: Apache License, Version 2.0
 *     https://www.apache.org/licenses/LICENSE-2.0
 *     
 */

int LINE_L2_Pin = A5;
int LINE_L1_Pin = A4;
int LINE_R1_Pin = A3;
int LINE_R2_Pin = A2;
int LED_Pin = D13;
int BUZZER_Pin = D2;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_Pin,OUTPUT);  
  pinMode(BUZZER_Pin, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.printf("\n\r LL2=%d LL1=%d LR1=%d LR2=%d",
    analogRead(LINE_L2_Pin),analogRead(LINE_L1_Pin),analogRead(LINE_R1_Pin),analogRead(LINE_R2_Pin));
  digitalWrite(LED_Pin,HIGH);
  if(analogRead(LINE_L1_Pin)>500){
    digitalWrite(BUZZER_Pin,HIGH);
  }
  else{
    digitalWrite(BUZZER_Pin,LOW);
  }
  delay(100);
  digitalWrite(LED_Pin,LOW);
  delay(100);
 }
