/*
 *    Arduino_Exercises/practice3.ino
 *    
 *    Copyright(C) 2020 RT Corporation <support@rt-net.jp>
 *      All rights reserved.
 *      
 *    License: Apache License, Version 2.0
 *     https://www.apache.org/licenses/LICENSE-2.0
 *     
 */

int LED_Pin = D13;
int BUZZER_Pin = D2;
int SW1_Pin = D7;
int SW2_Pin = D8;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_Pin,OUTPUT);
  pinMode(BUZZER_Pin,OUTPUT);
  pinMode(SW1_Pin,INPUT_PULLUP);
  pinMode(SW2_Pin,INPUT_PULLUP);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(SW1_Pin)==HIGH){
    digitalWrite(LED_Pin,HIGH);
  }else{
    digitalWrite(LED_Pin,LOW);    
  }
  if(digitalRead(SW2_Pin)==LOW){
    digitalWrite(BUZZER_Pin,HIGH);
  }else{
    digitalWrite(BUZZER_Pin,LOW);
  }
}
