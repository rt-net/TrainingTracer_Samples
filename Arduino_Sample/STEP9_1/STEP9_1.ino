/*
 *    Arduino_Sample/STEP9_1/STEP9_1.ino
 *    
 *    Copyright(C) 2020 RT Corporation <support@rt-net.jp>
 *    Copyright(C) 2020 M.Hirai
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

int POWER_Pin = A6;
int LINE_L2_Pin = A5;
int LINE_L1_Pin = A4;
int LINE_R1_Pin = A3;
int LINE_R2_Pin = A2;

int LED_Pin = D13;
int BUZZER_Pin = D2;

// 変数宣言
long  Line_Controll;
int inside_offset = 0;
int outside_offset = 0;
int L2_Value = 0;
int L1_Value = 0;
int R1_Value = 0;
int R2_Value = 0;
int ML_Value = 0;
int MR_Value = 0;


int CW_R  = LOW;
int CCW_R = HIGH;
int CW_L = HIGH;
int CCW_L = LOW;
int PWM_R_Value = 0;
int PWM_L_Value = 0;

//ラインが白の場合:1、ラインが黒の場合-1
int Line_signed = 1;

//0:マーカー検出、クロス検出がされていない
//1:Startマーカーを検出
//2:Startマーカーを通過中
//3:マーカー検出、クロス検出されてない
//4:クロスライン通過中
//5:マーカー検出、クロス検出されてない
//6:ダミーマーカー通過中
//7:ゴールマーカー検出
int line_State = 0;

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

void debug_AD() {
  Serial.printf("\n\r %d ML=%d,L2=%d L1=%d R1=%d,R2=%d,MR=%d",
                line_State, ML_Value, L2_Value, L1_Value, R1_Value, R2_Value, MR_Value);
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

void setup() {
  // put your setup code here, to run once:
  //IOポート設定
  pinMode(BUZZER_Pin, OUTPUT);
  pinMode(LED_Pin, OUTPUT);
  pinMode(SW1_Pin, INPUT_PULLUP);
  pinMode(SW2_Pin, INPUT_PULLUP);
  pinMode(DIR_R_Pin, OUTPUT);
  pinMode(DIR_L_Pin, OUTPUT);

  LED_DRIVE(2, 100, 100);
  Serial.begin(115200);

  while (1) {
    if (digitalRead(SW2_Pin) == LOW) {
      BUZZER_DRIVE(1, 100, 100);
      while (1) {
        get_AD();
        Serial.printf("\n\r VDD=%dmV LL2=%d LL1=%d LR1=%d LR2=%d inside_offset=%d outside_offset=%d ML=%d MR=%d xline=%d  marker_check=%d CTRL=%d",
                      analogRead(POWER_Pin) * 9677 / 1000,
                      L2_Value, L1_Value, R1_Value, R2_Value,
                      (L1_Value - R1_Value), (L2_Value - R2_Value),
                      adc_read_value(PB_1, 10), adc_read_value(PB_0, 10),
                      L2_Value + R1_Value + L1_Value + R2_Value,
                      MarkerCheck(),
                      L1_Value - R1_Value - inside_offset + 2 * (L2_Value - R2_Value - outside_offset)
                     );
        delay(100);
        if (digitalRead(SW2_Pin) == LOW) {
          delay(200);
          break;
        }
      }
    }

    //左のスイッチを押したら、走行開始
    if (digitalRead(SW1_Pin) == LOW) {
//      digitalWrite(LED_Pin, HIGH);
      get_AD();
      BUZZER_DRIVE(2, 70, 70);
      break;
    }
  }
}


void loop() {
  // put your main code here, to run repeatedly:

  get_AD();
  //マーカーを検出?
  if (Line_signed == 1 ) {
    MarkerCheck();
    if (line_State == 1) {
      BUZZER_DRIVE(1, 50, 50);
    } else if (line_State == 7) {
      BUZZER_DRIVE(2, 50, 50);
      RUN_STOP();
    }
  } else {
    if (adc_read_value(PB_0, 10) < 30) {
      RUN_STOP();
    }
  }
  //ラインセンサの値から制御量を算出する
  Line_Controll = (L1_Value - R1_Value - inside_offset) + 2 * (L2_Value - R2_Value - outside_offset);

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
}
