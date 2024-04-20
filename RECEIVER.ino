#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>

typedef struct struct_message {
  int up, down, right, left, triangle, circle, cross, square;
} struct_message;

struct_message myData;

Servo myservo1, myservo2;
const int SERVOS = 2;
int Left_motor = 34;
int Left_motor_pwm = 35;
int Right_motor = 32;
int Right_motor_pwm = 33;
int enA = 25;
int enB = 26;

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  if (myData.up == HIGH) {
    forward();
    Serial.println("chạy thẳng");
  } else
    stop();
  if (myData.down == HIGH) {
    back();
    Serial.println("chạy lùi");
  } else
    stop();
  if (myData.right == HIGH) {
    right();
    Serial.println("rẽ phải");
  } else
    stop();
  if (myData.left == HIGH) {
    left();
    Serial.println("rẽ trái");
  } else
    stop();
  if (myData.triangle == HIGH) {
    servo_triangle();
    Serial.println("nâng càng");
  }
  if (myData.cross == HIGH) {
    servo_cross();
    Serial.println("hạ càng");
  }
  if (myData.square == HIGH) {
    servo_square();
    Serial.println("mở càng");
  }
  if (myData.circle == HIGH) {
    servo_circle();
    Serial.println("đóng càng");
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);

  myservo1.attach(12);
  myservo2.attach(14);
  pinMode(34, OUTPUT);
  pinMode(35, OUTPUT);
  pinMode(32, OUTPUT);
  pinMode(33, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(26, OUTPUT);
  myservo1.write(20);
  myservo1.write(90);
}

void loop() {
}

void forward() {
  analogWrite(25, 220);
  digitalWrite(34, HIGH);
  digitalWrite(35, LOW);
  analogWrite(26, 220);
  digitalWrite(32, LOW);
  digitalWrite(33, HIGH);
}
void right() {
  analogWrite(25, 200);
  digitalWrite(34, HIGH);
  digitalWrite(35, LOW);
  analogWrite(25, 200);
  digitalWrite(32, HIGH);
  digitalWrite(33, LOW);
}
void back() {
  analogWrite(26, 220);
  digitalWrite(34, LOW);
  digitalWrite(35, HIGH);
  analogWrite(26, 220);
  digitalWrite(32, HIGH);
  digitalWrite(33, LOW);
}
void left() {
  analogWrite(25, 200);
  digitalWrite(34, LOW);
  digitalWrite(35, HIGH);
  analogWrite(26, 200);
  digitalWrite(32, LOW);
  digitalWrite(33, HIGH);
}
void stop() {
  digitalWrite(34, LOW);
  digitalWrite(35, LOW);
  digitalWrite(32, LOW);
  digitalWrite(33, LOW);
}
void servo_triangle() {
  myservo1.write(90);
}
void servo_cross() {
  myservo1.write(20);
}
void servo_square() {
  myservo1.write(90);
}
void servo_circle() {
  myservo1.write(0);
}
