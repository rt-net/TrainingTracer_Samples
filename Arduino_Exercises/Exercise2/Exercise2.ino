/*
 *    Arduino_Exercises/practice2.ino
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

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_Pin,OUTPUT);
  pinMode(BUZZER_Pin,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_Pin,LOW);//LEDを消灯
  digitalWrite(BUZZER_Pin,HIGH);
  delay(1000);                //1秒待つ
  digitalWrite(LED_Pin,HIGH);//LED点灯
  digitalWrite(BUZZER_Pin,LOW);
  delay(1000);                //1秒待つ
}
