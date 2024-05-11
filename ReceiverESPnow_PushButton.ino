#include <esp_now.h>
#include <WiFi.h>
#include <String.h>
#include <ESP32Servo.h>

Servo servo1;
Servo servo2;

int var1, var2, var3;
int Left_motor = 22;
int Left_motor_pwm = 23;
int Right_motor = 32;
int Right_motor_pwm = 33;
int enA = 14;   //PHAI
int enB = 15; //TRAI
const int pinLai = 16;
const int pingas = 17;
unsigned long lastDataReceivedTime = 0;  // Biến để lưu thời gian cuối cùng nhận được dữ liệu

struct __attribute__((packed)) dataPacket {
  String receivedState;
};

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  dataPacket packet;
  memcpy(&packet, incomingData, sizeof(packet));
  Serial.print("Received State: ");
  Serial.println(packet.receivedState);
  parseData(packet.receivedState);
  lastDataReceivedTime = millis();  // Lưu thời gian nhận được dữ liệu
                                    //digitalWrite(lightME,(packet.state));
}

void parseData(String data) {
  // Tìm vị trí của dấu '/'
  int firstSlashIndex = data.indexOf('/');
  int secondSlashIndex = data.indexOf('/', firstSlashIndex + 1);


  if (firstSlashIndex != -1 && secondSlashIndex != -1) {
    // Tách chuỗi thành 3 phần dựa trên dấu '/'
    String aStr = data.substring(0, firstSlashIndex);
    String bStr = data.substring(firstSlashIndex + 1, secondSlashIndex);
    String cStr = data.substring(secondSlashIndex + 1);

    // Chuyển đổi các chuỗi thành các số nguyên
    var1 = aStr.toInt();
    var2 = bStr.toInt();
    var3 = cStr.toInt();
  } else {
    // Xử lý trường hợp không tìm thấy dấu '/'
    var1 = 0;
    var2 = 90;
    var3 = 90;
  }
}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);  // Khởi tạo Serial để giao tiếp với máy tính qua USB
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);

  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);
  pinMode(32, OUTPUT);
  pinMode(33, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  servo2.attach(pingas);
  servo1.attach(pinLai);
}

void loop() {
  // Kiểm tra xem đã quá 5 giây từ lần cuối nhận được dữ liệu hay không
  if (millis() - lastDataReceivedTime >= 50) {
    var1 = 0;
    var2 = 90;
    var3 = 90;
  }
	servo1.write(var3);
  servo2.write(var2);   
  if (var1 == 0) {
    stop();
    Serial.println("dung lai");
  }
  if (var1 == 1) {
    forward();
    Serial.println("chay thang");
  }
  if (var1 == 2) {
    back();
    Serial.println("chay lui");
  }
  if (var1 == 3) {

    right();
    Serial.println("re phai");
  }
  if (var1 == 4) {
    left();
    Serial.println("re trai");
  }
  Serial.print(var1);
  Serial.print("\t");
  Serial.print(var2);
  Serial.print("\t");
  Serial.println(var3);
}
void forward() {
  analogWrite(14, 190);
  digitalWrite(22, LOW);
  digitalWrite(23, HIGH);
  analogWrite(15, 190);
  digitalWrite(32, HIGH);
  digitalWrite(33, LOW);
}
void right() {
  analogWrite(14, 110);
  digitalWrite(22, LOW);
  digitalWrite(23, HIGH);
  analogWrite(15, 255);
  digitalWrite(32, LOW);
  digitalWrite(33, HIGH);
}
void back() {
  analogWrite(14, 255);
  digitalWrite(22, HIGH);
  digitalWrite(23, LOW);
  analogWrite(15, 255);
  digitalWrite(32, LOW);
  digitalWrite(33, HIGH);
}
void left() {
  analogWrite(14, 255);
  digitalWrite(22, HIGH);
  digitalWrite(23, LOW);
  analogWrite(15, 110);
  digitalWrite(32, HIGH);
  digitalWrite(33, LOW);
}
void stop() {
  analogWrite(14, 0);
  digitalWrite(22, LOW);
  digitalWrite(23, LOW);
  analogWrite(15, 0);
  digitalWrite(32, LOW);
  digitalWrite(33, LOW);
}
