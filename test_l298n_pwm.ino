#define in1 7
#define in2 6
#define in3 5
#define in4 4
#define ena 9
#define enb 10


void setup() {
//đặt trạng thái output cho các chân
  pinMode(ena, OUTPUT);
  pinMode(enb, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  Serial.begin(9600);
}

void loop() {
//băm xung từ 30 để kiểm tra động cơ 2 bên có đều hay không, nếu không thì bù xung vào
  for (int initial_speed = 30;initial_speed<255;initial_speed++){
      analogWrite(ena, initial_speed+3);
      analogWrite(enb, initial_speed);
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
      Serial.println(initial_speed);
      delay(1000);
  }
}
