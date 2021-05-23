#include <Arduino.h>

int BLDC[3][2] = {{3, 9}, {5, 6}, {7, 8}};  //ピン番号

unsigned long delayTime = 10000;
int power = 255;
int count = 0;

void step(int _step, int _power, unsigned long _timer);

void setup(void) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 2; j++) {
      pinMode(BLDC[i][j], OUTPUT);
    }
  }

  TCCR2B = (TCCR2B & 0b11111000) | 0x01;  // 31.37255 [kHz]
  TCCR3B = (TCCR3B & 0b11111000) | 0x01;  // 31.37255 [kHz]
  TCCR4B = (TCCR4B & 0b11111000) | 0x01;  // 31.37255 [kHz]

  analogWrite(3, 0);
  analogWrite(5, 0);
  analogWrite(7, 0);
  digitalWrite(4, HIGH);  // HIGHで通電 LOWでハイインピーダンス
  digitalWrite(6, HIGH);
  digitalWrite(8, HIGH);

  power = 0;
  step(0, 0, 0);

  delay(3000);  //安全対策
}

void loop(void) {
  delayTime = 10000;

  //始動処理
  power = 40;
  for (int i = 0; i < 6; i++) {
    step(count, power, delayTime);
    count++;
    count %= 6;
  }
  //始動ここまで

  power = 17;
  for (int i = 0; i < 200; i++) {
    step(count, power, delayTime);
    count++;
    count %= 6;
  }

  delayTime = 5000;
  power = 27;
  for (int i = 0; i < 600; i++) {
    step(count, power, delayTime);
    count++;
    count %= 6;
  }

  delayTime = 2000;
  power = 150;
  for (int i = 0; i < 840; i++) {
    step(count, power, delayTime);
    count++;
    count %= 6;
  }

  power = 255;
  delayTime = 1300;
  for (int i = 0; i < 5600; i++) {
    step(count, power, delayTime);
    count++;
    count %= 6;
  }

  step(count, 0, 0);  //停止

  delay(3000);
}

void step(int _step, int _power, unsigned long _timer) {
  switch (_step) {
    case 0:                             // U→W
      analogWrite(BLDC[0][0], _power);  // U_IN
      analogWrite(BLDC[1][0], 0);       // V_IN
      analogWrite(BLDC[2][0], 0);       // W_IN

      digitalWrite(BLDC[0][1], HIGH);  // V_SD
      digitalWrite(BLDC[1][1], LOW);   // V_IN
      digitalWrite(BLDC[2][1], HIGH);  // W_IN
      break;

    case 1:  // U→V
      analogWrite(BLDC[0][0], _power);
      analogWrite(BLDC[1][0], 0);
      analogWrite(BLDC[2][0], 0);

      digitalWrite(BLDC[0][1], HIGH);
      digitalWrite(BLDC[1][1], HIGH);
      digitalWrite(BLDC[2][1], LOW);
      break;

    case 2:  // W→V
      analogWrite(BLDC[0][0], 0);
      analogWrite(BLDC[1][0], 0);
      analogWrite(BLDC[2][0], _power);

      digitalWrite(BLDC[0][1], LOW);
      digitalWrite(BLDC[1][1], HIGH);
      digitalWrite(BLDC[2][1], HIGH);
      break;

    case 3:  // W→U
      analogWrite(BLDC[0][0], 0);
      analogWrite(BLDC[1][0], 0);
      analogWrite(BLDC[2][0], _power);

      digitalWrite(BLDC[0][1], HIGH);
      digitalWrite(BLDC[1][1], LOW);
      digitalWrite(BLDC[2][1], HIGH);
      break;

    case 4:  // V→U
      analogWrite(BLDC[0][0], 0);
      analogWrite(BLDC[1][0], _power);
      analogWrite(BLDC[2][0], 0);

      digitalWrite(BLDC[0][1], HIGH);
      digitalWrite(BLDC[1][1], HIGH);
      digitalWrite(BLDC[2][1], LOW);
      break;

    case 5:  // V→W
      analogWrite(BLDC[0][0], 0);
      analogWrite(BLDC[1][0], _power);
      analogWrite(BLDC[2][0], 0);

      digitalWrite(BLDC[0][1], LOW);
      digitalWrite(BLDC[1][1], HIGH);
      digitalWrite(BLDC[2][1], HIGH);
      break;
  }

  delayMicroseconds(_timer);
}