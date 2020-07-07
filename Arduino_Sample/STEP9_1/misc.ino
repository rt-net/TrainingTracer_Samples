/*
 *    Arduino_Sample/STEP9_1/misc.ino
 *    
 *    Copyright(C) 2020 RT Corporation <support@rt-net.jp>
 *      All rights reserved.
 *      
 *    License: Apache License, Version 2.0
 *     https://www.apache.org/licenses/LICENSE-2.0
 *     
 */

void LED_DRIVE(char times, int high_ms, int low_ms) {
  for (char i = 0; i < times; i++) {
    digitalWrite(LED_Pin, HIGH);
    delay(high_ms);
    digitalWrite(LED_Pin, LOW);
    delay(low_ms);
  }
}

void BUZZER_DRIVE(char times, int high_ms, int low_ms) {
  for (char i = 0; i < times; i++) {
    digitalWrite(BUZZER_Pin, HIGH);   // ブザー鳴らす
    delay(100);                       // 指定時間待ち
    digitalWrite(BUZZER_Pin, LOW);    // ブザー止める
    delay(100);                       // 指定時間待ち
  }
}

void RUN_STOP(void) {
  digitalWrite(LED_Pin, LOW);
  analogWrite(PWM_L_Pin, 0);
  analogWrite(PWM_R_Pin, 0);
  digitalWrite(BUZZER_Pin, HIGH);
  delay(100);
  digitalWrite(BUZZER_Pin, LOW);
  line_State=0;
  while (1) {
    if (digitalRead(SW1_Pin) == LOW) {
//      digitalWrite(LED_Pin, HIGH);
      break;
    }
  }
}
