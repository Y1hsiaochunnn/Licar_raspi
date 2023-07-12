#include <SCServo.h>
SMS_STS st;

#define S_RXD 18
#define S_TXD 19

const int gpio5 = 5;
const int gpio4 = 4;

const int gpio27 = 27;
const int gpio16 = 16;
const int threshold = 10;

const int motor1Id = 1;           // 1号电机ID
const int motor3Id = 3;           // 3号电机ID
const int motor1MinAngle = 0;     // 1号电机最小角度
const int motor1MaxAngle = 4095;  // 1号电机最大角度
const int motor3MinAngle = 0;     // 3号电机最小角度
const int motor3MaxAngle = 4095;  // 3号电机最大角度

const uint16_t PWMA = 25;
const uint16_t AIN2 = 17;
const uint16_t AIN1 = 21;
const uint16_t BIN1 = 22;
const uint16_t BIN2 = 23;
const uint16_t PWMB = 26;

const uint16_t ANALOG_WRITE_BITS = 8;

int freq = 100000;
int channel_A = 0;
int channel_B = 1;
int resolution = ANALOG_WRITE_BITS;

void initMotors() {
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);

  ledcSetup(channel_A, freq, resolution);
  ledcAttachPin(PWMA, channel_A);

  ledcSetup(channel_B, freq, resolution);
  ledcAttachPin(PWMB, channel_B);
}

void forwardA(uint16_t pwm) {
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  ledcWrite(channel_A, pwm);
}

void backwardA(uint16_t pwm) {
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  ledcWrite(channel_A, pwm);
}

void forwardB(uint16_t pwm) {
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  ledcWrite(channel_B, pwm);
}

void backwardB(uint16_t pwm) {
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  ledcWrite(channel_B, pwm);
}

void stopMotors() {
  ledcWrite(channel_A, 0);
  ledcWrite(channel_B, 0);
}

void setup() {
  Serial.begin(9600);
  Serial1.begin(1000000, SERIAL_8N1, S_RXD, S_TXD);
  st.pSerial = &Serial1;
  pinMode(gpio5, INPUT);
  pinMode(gpio4, INPUT);
  pinMode(gpio27, INPUT);
  pinMode(gpio16, INPUT);
  initMotors();
}

void loop() {
  int pwmValue27 = pulseIn(gpio27, HIGH);
  int pwmValue16 = pulseIn(gpio16, HIGH);
  int pwmValue5 = pulseIn(gpio5, HIGH);
  int pwmValue4 = pulseIn(gpio4, HIGH);

  int motor3Angle = map(pwmValue5, 1000, 2000, motor3MinAngle, motor3MaxAngle);
  int motor1Angle = map(pwmValue4, 1000, 2000, motor1MinAngle, motor1MaxAngle);
  if (pwmValue5 > 10) {
    st.WritePosEx(motor1Id, motor3Angle, 3073, 50);
  }
  if (pwmValue5 > 10) {
    st.WritePosEx(motor3Id, motor1Angle, 3073, 50);
  }

  if (pwmValue27 > 100) {
    if (abs(pwmValue27 - 1500) > threshold) {
      if (pwmValue27 > 1500) {
        // 控制电机A向前转动
        int speed = map(pwmValue27, 1500, 2000, 0, 255);  // 将PWM值映射到速度范围
        forwardA(speed);
      } else {
        // 控制电机A向后转动
        int speed = map(pwmValue27, 1500, 1000, 0, 255);  // 将PWM值映射到速度范围
        backwardA(speed);
      }
    } else {
      // 停止电机A
      ledcWrite(channel_A, 0);
    }
  } else {
    // 停止电机A
    ledcWrite(channel_A, 0);
  }

  if (pwmValue16 > 100) {
    if (abs(pwmValue16 - 1500) > threshold) {
      if (pwmValue16 > 1500) {
        // 控制电机B向前转动
        int speed = map(pwmValue16, 1500, 2000, 0, 255);  // 将PWM值映射到速度范围
        forwardB(speed);
      } else {
        // 控制电机B向后转动
        int speed = map(pwmValue16, 1500, 1000, 0, 255);  // 将PWM值映射到速度范围
        backwardB(speed);
      }
    } else {
      // 停止电机B
      ledcWrite(channel_B, 0);
    }
  } else {
    // 停止电机B
    ledcWrite(channel_B, 0);
  }
}
