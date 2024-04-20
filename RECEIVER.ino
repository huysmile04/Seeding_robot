#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>

typedef struct struct_message {
  int up, down, right, left, triangle, circle, cross, square;
} struct_message;

struct_message myData;

Servo myservo1, myservo2;
const int SERVOS = 2;
int Left_motor = 22;
int Left_motor_pwm = 23;
int Right_motor = 32;
int Right_motor_pwm = 33;
int enA = 25;
int enB = 26;

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.println(myData.up);
  if (myData.up == 1) {
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
  } else
    Serial.println("Initialazed with success");
  esp_now_register_recv_cb(OnDataRecv);
  Serial.print("state of up:");
  Serial.println(myData.up);
  myservo1.attach(12);
  myservo2.attach(14);
  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);
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
  digitalWrite(22, HIGH);
  digitalWrite(23, LOW);
  analogWrite(26, 220);
  digitalWrite(32, LOW);
  digitalWrite(33, HIGH);
}
void right() {
  analogWrite(25, 200);
  digitalWrite(22, HIGH);
  digitalWrite(23, LOW);
  analogWrite(25, 200);
  digitalWrite(32, HIGH);
  digitalWrite(33, LOW);
}
void back() {
  analogWrite(26, 220);
  digitalWrite(22, LOW);
  digitalWrite(23, HIGH);
  analogWrite(26, 220);
  digitalWrite(32, HIGH);
  digitalWrite(33, LOW);
}
void left() {
  analogWrite(25, 200);
  digitalWrite(22, LOW);
  digitalWrite(23, HIGH);
  analogWrite(26, 200);
  digitalWrite(32, LOW);
  digitalWrite(33, HIGH);
}
void stop() {
  analogWrite(25, 0);
  digitalWrite(22, LOW);
  digitalWrite(23, LOW);
  analogWrite(26, 0);
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
