#include <WiFi.h>
#include <esp_now.h>

// Thiết lập setup


// Thiết lập tín hiệu điều khiển
struct ControlSignal {

};

ControlSignal Signal;

// Hàm xử lý khi nhận được dữ liệu
void onDataReceived(const uint8_t *mac, const uint8_t *data, int len) {
  if (len == sizeof(ControlSignal)) {
    ControlSignal controlSignal;
    memcpy(&Signal, data, sizeof(Signal));

    // Điều khiển xe dựa trên dữ liệu nhận được

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
}

void loop() {
  
}
