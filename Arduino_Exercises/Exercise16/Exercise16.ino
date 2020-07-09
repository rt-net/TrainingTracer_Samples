/*
 *    Arduino_Exercises/practice16.ino
 *    
 *    Copyright(C) 2020 RT Corporation <support@rt-net.jp>
 *      All rights reserved.
 *      
 *    License: Apache License, Version 2.0
 *     https://www.apache.org/licenses/LICENSE-2.0
 *     
 */

//ピンの設定
int DIR_R_Pin = D12;
int DIR_L_Pin = D0;
int PWM_R_Pin = D11;
int PWM_L_Pin = D10;

int SW1_Pin = D7;
int SW2_Pin = D8;

int LINE_L2_Pin = A5;
int LINE_L1_Pin = A4;
int LINE_R1_Pin = A3;
int LINE_R2_Pin = A2;

int LED_Pin = D13;
int BUZZER_Pin = D2;

// 変数宣言
long  Line_Controll;
int inside_offset = -64;
int outside_offset = -30;

int CW_R  = 0;
int CCW_R = 1;
int CW_L = 1;
int CCW_L = 0;
int PWM_R_Value = 0;
int PWM_L_Value = 0;

//ラインが白の場合:1、ラインが黒の場合-1
int Line_signed = -1;

void LED_DRIVER(char n, int on_ms, int off_ms){
  for(char i = 0; i < n ;i++){
    digitalWrite(LED_Pin, HIGH);
    delay(on_ms);
    digitalWrite(LED_Pin, LOW);
    delay(off_ms);
  }
}

void BUZZER_DRIVER(char n, int on_ms, int off_ms){
  for(char i = 0; i < n ;i++){
    digitalWrite(BUZZER_Pin, HIGH);
    delay(on_ms);
    digitalWrite(BUZZER_Pin, LOW);
    delay(off_ms);
  }
}

void setup() {
  // put your setup code here, to run once:
  //IOポート設定
  pinMode(BUZZER_Pin, OUTPUT);
  pinMode(LED_Pin, OUTPUT);
  pinMode(SW1_Pin, INPUT_PULLUP);
  pinMode(SW2_Pin, INPUT_PULLUP);
  pinMode(DIR_R_Pin, OUTPUT);
  pinMode(DIR_L_Pin, OUTPUT);

  LED_DRIVER(2,100,100);

  Serial.begin(115200);

  while (1) {

    if (digitalRead(SW2_Pin) == LOW) {
      BUZZER_DRIVER(1,100,100);
      while (1) {
        Serial.printf("\n\r LL2=%d LL1=%d LR1=%d LR2=%d inside_Offset=%d,outside_offset=%d",
                      analogRead(LINE_L2_Pin), analogRead(LINE_L1_Pin), analogRead(LINE_R1_Pin), analogRead(LINE_R2_Pin),
                      (analogRead(LINE_L1_Pin) - analogRead(LINE_R1_Pin)), (analogRead(LINE_L2_Pin) - analogRead(LINE_R2_Pin)));
        delay(100);
        if (digitalRead(SW2_Pin) == LOW) {
          delay(200);
          break;
        }
      }
    }

    //左のスイッチを押したら、走行開始
    if (digitalRead(SW1_Pin) == LOW) {
      digitalWrite(LED_Pin, HIGH);
      BUZZER_DRIVER(2,100,70);
      break;
    }
  }
}
void loop() {
  // put your main code here, to run repeatedly:
  //ラインセンサの値から制御量を算出する

  Line_Controll = (analogRead(LINE_L1_Pin) - analogRead(LINE_R1_Pin) - inside_offset)
                  + 2 * (analogRead(LINE_L2_Pin) - analogRead(LINE_R2_Pin) - outside_offset);

  PWM_L_Value = 80 - Line_signed * Line_Controll / 10;
  PWM_R_Value = 80 + Line_signed * Line_Controll / 10;
  //左モーターPWM出力
  if (PWM_L_Value < 0) {
    digitalWrite(DIR_L_Pin, CCW_L);//モーター後進設定
  } else {
    digitalWrite(DIR_L_Pin, CW_L);//モーター前進設定
  }
  PWM_L_Value = abs(PWM_L_Value);
  if (PWM_L_Value > 255) {
    PWM_L_Value = 255; //モーター制御値上下ガード処理
  }
  if (PWM_L_Value <= 0) {
    PWM_L_Value = 0; //モーター制御値上下ガード処理
  }
  analogWrite(PWM_L_Pin, PWM_L_Value);

  //右モーターPWM出力
  if (PWM_R_Value < 0) {
    digitalWrite(DIR_R_Pin, CCW_R);//モーター後進設定
  } else {
    digitalWrite(DIR_R_Pin, CW_R);//モーター前進設定
  }
  PWM_R_Value = abs(PWM_R_Value);
  if (PWM_R_Value > 255) {
    PWM_R_Value = 255; //モーター制御値上下ガード処理
  }
  if (PWM_R_Value <= 0) {
    PWM_R_Value = 0; //モーター制御値上下ガード処理
  }
  analogWrite(PWM_R_Pin, PWM_R_Value);

  delay(1);
  //PWM、コントロールの値確認用
  //  Serial.printf("\n\r PWM_R=%d PWM_L=%d CONT=%d",PWM_R_Value,PWM_L_Value,Line_Controll);
  //  delay(100);

}
