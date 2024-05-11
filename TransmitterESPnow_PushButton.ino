#include <esp_now.h>
#include <WiFi.h>
#include <String.h>

// const int Lai = 34, Gas = 35,
const int ledPin = 17;
// int var1 = 90, var2 = 110;
const int down = 16;  // Pin của nút ấn 1
const int up = 18;    // Pin của nút ấn 2
const int right = 4;  // Pin của nút ấn 3
const int left = 15;  // Pin của nút ấn 4
const int dong = 13;  // Pin của nút ấn 5
const int ha = 12;    // Pin của nút ấn 6
const int nang = 14;  // Pin của nút ấn 7
const int mo = 27;    // Pin của nút ấn 8
//const int up = 2, down = 16, right = 4, left = 15, nang = 14, ha = 12, mo = 27, dong = 13;
int var, var1, var2;
// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = { 0xCC, 0x7B, 0x5C, 0x27, 0xAF, 0x88 };
struct __attribute__((packed)) dataPacket {
  String state;
};

esp_now_peer_info_t peerInfo;

int Down = 0;   // Pin của nút ấn 1
int Up = 0;     // Pin của nút ấn 2
int Right = 0;  // Pin của nút ấn 3
int Left = 0;   // Pin của nút ấn 4
int Dong = 0;   // Pin của nút ấn 5
int Ha = 0;     // Pin của nút ấn 6
int Nang = 0;   // Pin của nút ấn 7
int Mo = 0;     // Pin của nút ấn 8

// callback when data is sent - I CAN CHANGE THIS FUNCTION BELOW
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  if (status == ESP_NOW_SEND_SUCCESS) {
    digitalWrite(ledPin, 0);
  } else {
    digitalWrite(ledPin, 1);
    delay(30);
    digitalWrite(ledPin, 0);
    delay(30);
  }

  //Serial.print("\r\nLast Packet Send Status:\t");
  //Serial.print(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(up, INPUT);
  pinMode(down, INPUT);
  pinMode(right, INPUT);
  pinMode(left, INPUT);
  pinMode(nang, INPUT);
  pinMode(ha, INPUT);
  pinMode(mo, INPUT);
  pinMode(dong, INPUT);

  digitalWrite(ledPin, 1);
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  // int LaiValue = analogRead(Lai);
  // int GasValue = analogRead(Gas);
  // var1 = map(LaiValue, 0, 3800, 55, 125);
  // var2 = map(GasValue, 150, 3600, 15, 100);
  // if (var1 >= 120){
  //   var1 = 120;
  // }
  Up = digitalRead(up);
  Down = digitalRead(down);
  Right = digitalRead(right);
  Left = digitalRead(left);
  Nang = digitalRead(nang);
  Ha = digitalRead(ha);
  Mo = digitalRead(mo);
  Dong = digitalRead(dong);


  if (Up == 1) {
    var = 1;
  } else if (Down == 1) {
    var = 2;
  } else if (Right == 1) {
    var = 3;
  } else if (Left == 1) {
    var = 4;
  } else
    var = 0;
  if (digitalRead(nang) == 1) {
    var1 = 90;
  }
  if (digitalRead(ha) == 1) {
    var1 = 0;
  }
  if (digitalRead(mo) == 1) {
    var2 = 25;
  }
  if (digitalRead(dong) == 1) {
    var2 = 90;
  }
  dataPacket packet;
  packet.state = String(var) + "/" + String(var1) + "/" + String(var2);
  //packet.state = digitalRead(pushDown);
  esp_now_send(broadcastAddress, (uint8_t *)&packet, sizeof(packet));
  Serial.print(var);
  Serial.print("\t");
  Serial.print(var1);
  Serial.print("\t");
  Serial.print(var2);
  Serial.print("\t");
  Serial.print("\n");

  delay(30);
}
