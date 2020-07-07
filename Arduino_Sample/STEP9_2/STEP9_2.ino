/*
 *    Arduino_Sample/STEP9_2/STEP9_2.ino
 *    
 *    Copyright(C) 2020 RT Corporation <support@rt-net.jp>
 *    Copyright(C) 2020 M.Hirai
 *      All rights reserved.
 *      
 *    License: Apache License, Version 2.0
 *     https://www.apache.org/licenses/LICENSE-2.0
 *     
 */

#include<HardwareTimer.h>
#include<Wire.h>

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

int ENC_A_R_Pin = A0;
int ENC_B_R_Pin = A1;
int ENC_A_L_Pin = D1;
int ENC_B_L_Pin = D9;

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

int time_tmp = 0;
int time_CNT = 0;
int sub_time;


//0:マーカー検出、クロス検出がされていない
//1:Startマーカーを検出
//2:Startマーカーを通過中
//3:マーカー検出、クロス検出されてない
//4:クロスライン通過中
//5:マーカー検出、クロス検出されてない
//6:ダミーマーカー通過中
//7:ゴールマーカー検出
int line_State = 0;

int Encoder_R_CNT = 0;
int Encoder_L_CNT = 0;

// センサーの値を保存するグローバル関数

float theta = 0;

void EncoderA_R() {
  if (digitalRead(ENC_A_R_Pin) == HIGH) {
    if (digitalRead(ENC_B_R_Pin) == LOW) {
      Encoder_R_CNT++;
    } else {
      Encoder_R_CNT--;
    }
  } else {
    if (digitalRead(ENC_B_R_Pin) == HIGH) {
      Encoder_R_CNT++;
    } else {
      Encoder_R_CNT--;
    }
  }
}

void EncoderB_R() {
  if (digitalRead(ENC_B_R_Pin) == HIGH) {
    if (digitalRead(ENC_A_R_Pin) == HIGH) {
      Encoder_R_CNT++;
    } else {
      Encoder_R_CNT--;
    }
  } else {
    if (digitalRead(ENC_A_R_Pin) == LOW) {
      Encoder_R_CNT++;
    } else {
      Encoder_R_CNT--;
    }
  }
}

void EncoderA_L() {
  if (digitalRead(ENC_A_L_Pin) == HIGH) {
    if (digitalRead(ENC_B_L_Pin) == LOW) {
      Encoder_L_CNT++;
    } else {
      Encoder_L_CNT--;
    }
  } else {
    if (digitalRead(ENC_B_L_Pin) == HIGH) {
      Encoder_L_CNT++;
    } else {
      Encoder_L_CNT--;
    }
  }
}

void EncoderB_L() {
  if (digitalRead(ENC_B_L_Pin) == HIGH) {
    if (digitalRead(ENC_A_L_Pin) == HIGH) {
      Encoder_L_CNT++;
    } else {
      Encoder_L_CNT--;
    }
  } else {
    if (digitalRead(ENC_A_L_Pin) == LOW) {
      Encoder_L_CNT++;
    } else {
      Encoder_L_CNT--;
    }
  }
}

void TimerCNT(HardwareTimer*) {
  time_CNT++;

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

  pinMode(ENC_A_R_Pin, INPUT);
  pinMode(ENC_B_R_Pin, INPUT);
  pinMode(ENC_A_L_Pin, INPUT);
  pinMode(ENC_B_L_Pin, INPUT);

  attachInterrupt(ENC_A_R_Pin, EncoderA_R, CHANGE);
  attachInterrupt(ENC_B_R_Pin, EncoderB_R, CHANGE);
  attachInterrupt(ENC_A_L_Pin, EncoderA_L, CHANGE);
  attachInterrupt(ENC_B_L_Pin, EncoderB_L, CHANGE);

  Wire.begin();
  BMX055_Init();
  delay(300);

  TIM_TypeDef *Instance = TIM2;
  HardwareTimer *Timer2 = new HardwareTimer(Instance);
  Timer2->setMode(1, TIMER_OUTPUT_COMPARE);
  Timer2->setOverflow(100, MICROSEC_FORMAT);
  Timer2->attachInterrupt(TimerCNT);
  Timer2->resume();

  LED_DRIVE(2, 100, 100);
  Serial.begin(115200);

  while (1) {
   if (digitalRead(SW2_Pin) == LOW) {
      BUZZER_DRIVE(1, 100, 100);
       time_tmp = time_CNT;      
      while (1) {
        get_AD();
        BMX055_Gyro();
        sub_time=time_CNT - time_tmp;
        theta = theta + get_zGyro() * (sub_time+1000)/10000;

        Serial.print(sub_time);Serial.print(" ");Serial.print(theta);Serial.print(" ");Serial.println(get_zGyro());
/*        
        Serial.printf("\n\r VDD=%dmV LL2=%d LL1=%d LR1=%d LR2=%d inside_offset=%d outside_offset=%d ML=%d MR=%d xline=%d  marker_check=%d CTRL=%d ENC_R=%d ENC_L=%d theta=%d zGyro=%d time_CNT=%d",
                      analogRead(POWER_Pin) * 9677 / 1000,
                      L2_Value, L1_Value, R1_Value, R2_Value,
                      (L1_Value - R1_Value), (L2_Value - R2_Value),
                      adc_read_value(PB_1, 10), adc_read_value(PB_0, 10),
                      L2_Value + R1_Value + L1_Value + R2_Value,
                      MarkerCheck(),
                      L1_Value - R1_Value - inside_offset + 2 * (L2_Value - R2_Value - outside_offset),
                      Encoder_R_CNT,
                      Encoder_L_CNT,
                      (int)(theta*10),
                      (int)(zGyro*10),
                      time_CNT
                     );
                     */
        delay(100);
       time_tmp = time_CNT;        
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
