#include <WiFi.h>
#include <esp_now.h>
#include <ESP32Servo.h>

// Thiết lập 
#define MOTOR_A_IN1 26
#define MOTOR_A_IN2 27
#define MOTOR_A_EN 14
#define MOTOR_B_IN1 32
#define MOTOR_B_IN2 33
#define MOTOR_B_EN 15
#define SERVO0 12
#define SERVO1 13


static  Servo Servo_0; // lên xuống
static  Servo Servo_1; // gắp thả

// Thiết lập tín hiệu điều khiển
struct ControlSignal {
  int forward;
  int backward;
  int turn_left;
  int turn_right;
  int up;
  int down;
  int hold;
  int drop;
};

void forward() {
  digitalWrite(MOTOR_A_IN1, HIGH);
  digitalWrite(MOTOR_A_IN2, LOW);
  digitalWrite(MOTOR_B_IN1, HIGH);
  digitalWrite(MOTOR_B_IN2, LOW);
  analogWrite(MOTOR_A_EN, 255);
  analogWrite(MOTOR_B_EN, 255);
}

void backward() {
  digitalWrite(MOTOR_A_IN1, LOW);
  digitalWrite(MOTOR_A_IN2, HIGH);
  digitalWrite(MOTOR_B_IN1, LOW);
  digitalWrite(MOTOR_B_IN2, HIGH);
  analogWrite(MOTOR_A_EN, 255);
  analogWrite(MOTOR_B_EN, 255);
}

void turn_left() {
  digitalWrite(MOTOR_A_IN1, LOW);
  digitalWrite(MOTOR_A_IN2, LOW);
  digitalWrite(MOTOR_B_IN1, HIGH);
  digitalWrite(MOTOR_B_IN2, LOW);
  analogWrite(MOTOR_A_EN, 255);
  analogWrite(MOTOR_B_EN, 255);

}

void turn_right() {
  digitalWrite(MOTOR_A_IN1, HIGH);
  digitalWrite(MOTOR_A_IN2, LOW);
  digitalWrite(MOTOR_B_IN1, LOW);
  digitalWrite(MOTOR_B_IN2, LOW);
  analogWrite(MOTOR_A_EN, 255);
  analogWrite(MOTOR_B_EN, 255);

}

void stop() {
  digitalWrite(MOTOR_A_IN1, LOW);
  digitalWrite(MOTOR_A_IN2, LOW);
  digitalWrite(MOTOR_B_IN1, LOW);
  digitalWrite(MOTOR_B_IN2, LOW);

}

ControlSignal Signal;

// Hàm xử lý khi nhận được dữ liệu
void onDataReceived(const uint8_t *mac, const uint8_t *data, int len) {
  if (len == sizeof(ControlSignal)) {
    ControlSignal controlSignal;
    memcpy(&Signal, data, sizeof(Signal));

    if (Signal.forward == HIGH) {
      forward();
    }

    if (Signal.backward == HIGH) {
      backward();
    }

    if (Signal.turn_left == HIGH) {
      turn_left();
    }

    if (Signal.turn_right == HIGH) {
      turn_right();
    }

    if (Signal.forward == LOW && Signal.backward == LOW && Signal.turn_left == LOW && Signal.turn_right == LOW) {
      stop();
    }

    if(Signal.up == HIGH) {
      Servo_0.write(90);
    }

    if (Signal.down == HIGH) {
      Servo_0.write(0);
    } 

    if (Signal.hold == HIGH) {
      Servo_1.write(80);
    }

    if (Signal.drop == HIGH) {
      Servo_1.write(0);
    }
  }
}

void setup() {
  Serial.begin(115200);

  // Khởi tạo ESP Now
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP Now initialization failed");
    return;
  }

  // Đăng ký hàm xử lý dữ liệu nhận được
  esp_now_register_recv_cb(onDataReceived);

  pinMode(MOTOR_A_IN1, OUTPUT);
  pinMode(MOTOR_A_IN2, OUTPUT);
  pinMode(MOTOR_B_IN1, OUTPUT);
  pinMode(MOTOR_B_IN2, OUTPUT);
  pinMode(MOTOR_A_EN, OUTPUT);
  pinMode(MOTOR_B_EN, OUTPUT);

  Servo_0.attach(SERVO0);
  Servo_1.attach(SERVO1);
  Servo_0.write(0);
  Servo_1.write(0);

}

void loop() {
  
}
