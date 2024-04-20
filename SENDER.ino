#include <esp_now.h>
#include <WiFi.h>

uint8_t broadcastAddress[] = { 0xCC, 0x7B, 0x5C, 0x28, 0x36, 0xE0 };

#define UP 12
#define DOWN 14
#define RIGHT 27
#define LEFT 26
#define TRIANGLE 33
#define CIRCLE 32
#define CROSS 35
#define SQUARE 34
typedef struct struct_message {
  int up, down, right, left, triangle, circle, cross, square;
} struct_message;

struct_message myData;

esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  pinMode(12, INPUT);
  pinMode(14, INPUT);
  pinMode(27, INPUT);
  pinMode(26, INPUT);
  pinMode(33, INPUT);
  pinMode(32, INPUT);
  pinMode(35, INPUT);
  pinMode(34, INPUT);
}

void loop() {
  myData.up = digitalRead(UP);
  myData.down = digitalRead(DOWN);
  myData.right = digitalRead(RIGHT);
  myData.left = digitalRead(LEFT);
  myData.triangle = digitalRead(TRIANGLE);
  myData.circle = digitalRead(CIRCLE);
  myData.cross = digitalRead(CROSS);
  myData.square = digitalRead(SQUARE);

  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));

  if (result == ESP_OK) {
    Serial.println("Sent with success");
  } else {
    Serial.println("Error sending the data");
  }
  delay(500);
}
