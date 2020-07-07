/*
 *    Arduino_Sample/STEP9_2/misc.ino
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

void debug_AD() {
  Serial.printf("\n\r %d ML=%d,L2=%d L1=%d R1=%d,R2=%d,MR=%d",
                line_State, ML_Value, L2_Value, L1_Value, R1_Value, R2_Value, MR_Value);
}

void get_AD(void) {
  R2_Value = analogRead(LINE_R2_Pin);
  R1_Value = analogRead(LINE_R1_Pin);
  L1_Value = analogRead(LINE_L1_Pin);
  L2_Value = analogRead(LINE_L2_Pin) * 2;
  ML_Value = adc_read_value(PB_1, 10);
  MR_Value = adc_read_value(PB_0, 10);

  //床の反射率が一定ではないため、特定の箇所で調査し、平均化したオフセットを算出
  outside_offset = R2_Value * 0.1281 - 17.245;
  inside_offset = R2_Value * 0.1379 + 26.743;
}

int MarkerCheck(void) {
  //      debug_AD();
  if ((MR_Value > (R2_Value*0.169+2.7488+30))  && (MR_Value > 90)) { //マーカーセンサアクティブ
    if (line_State == 0) {
      line_State = 1;
      return 1;
    } else if (line_State == 1) {
      line_State = 2;
      return 2;
    } else if (line_State == 3) {
      line_State = 7;
      return 7;
    } else if (line_State == 4) {
      line_State = 6;
      return 6;
    } else if (line_State == 5) {
      line_State = 6;
      return 6;
    } else if(line_State == 6){
      line_State = 6;
      return 6;      
    }
  }

  if (line_State == 1) {
    line_State = 3;
    return 3;
  }

  if (line_State == 2) {
    line_State = 3;
    return 3;
  }

  if (line_State == 6) {
    line_State = 3;
    return 3;
  }

  if (line_State == 3) {
    if ((R2_Value + R1_Value + L1_Value + L2_Value) > 2500) { //クロスライン
      line_State = 4; //クロスラインを検出
      digitalWrite(LED_Pin,HIGH);
      return 4;
    }
  }
  if (line_State == 4) {
    if ((R2_Value + R1_Value + L1_Value + L2_Value) < 2000) {
      line_State = 5;
      digitalWrite(LED_Pin,LOW);
      return 5;
    }
  }

  return line_State;
}
