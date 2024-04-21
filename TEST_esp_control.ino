#include <WiFi.h>
#include <esp_now.h>

#define Forward 26
#define Backward 27
#define Right 32
#define Left 33
#define Up 34 // tam giác
#define Drop 35 // tròn
#define Down 36 // X
#define Hold 39 // vuông

// Địa chỉ MAC của ESP nhận
uint8_t receiverAddress[] = {1, 1, 1, 1, 1, 1};

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

// Hàm gửi tín hiệu điều khiển
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  Serial.print("Packet to: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}


void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  pinMode(Forward, INPUT);
  pinMode(Backward, INPUT);
  pinMode(Left, INPUT);
  pinMode(Right, INPUT);
  pinMode(Up, INPUT);
  pinMode(Down, INPUT);
  pinMode(Hold, INPUT);
  pinMode(Drop, INPUT);

  

  // Khởi tạo ESP Now
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP Now initialization failed");
    return;
  }

  // Đăng ký thiết bị nhận
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, receiverAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  // Thiết lập hàm gửi dữ liệu
  esp_now_register_send_cb(OnDataSent);
}

void loop() {
  // Khởi tạo biến tín hiệu điều khiển
  ControlSignal Signal;

  // Nhập tín hiệu từ tay cầm
  Signal.forward = digitalRead(Forward);
  Signal.backward = digitalRead(Backward);
  Signal.turn_right = digitalRead(Right);
  Signal.turn_left = digitalRead(Left);
  Signal.up = digitalRead(Up);
  Signal.drop = digitalRead(Drop);
  Signal.down = digitalRead(Down);
  Signal.hold = digitalRead(Hold);

  // Truyền dữ liệu
  esp_err_t result = esp_now_send(
    receiverAddress, 
    (uint8_t *) &Signal,
    sizeof(ControlSignal));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }

  delay(100);
}
