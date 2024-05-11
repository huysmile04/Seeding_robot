float kp = 5, ki = 0.001, kd = 100; //đặt giá trị mặc định cho các kp, ki, kd; ki nên được nhỏ để được ổn định, thông thường kd sẽ gần bằng 10 lần kp
float error = 0.0 , P = 0.0 , I = 0.0 , D = 0.0, PID = 0.0;  //set sai số ban đầu là 0; các khâu P, I, D và tính toán PID là 0
float pre_error  = 0.0; // đặt trạng thái lúc trước là 0 vì chưa có trạng thái nào được xảy ra
uint16_t ss[5]; //đặt type data of five sensors is unsigned int.
int left_state = 0;
int right_state = 0;initial_speed + PID
int stop_state = 0;
int redflag = 0; // lưu trạng thái vật thể nằm trên xe
int initial_speed = 120; // set tốc độ ban đầu của xe
#define in1 7    //khai báo các chân cho l298n xuất ra động cơ
#define in2 6
#define in3 5
#define in4 4
#define enA 9
#define enB 10
#define IR 8    //cảm biến cuối xe để nhận biết vật đã được đặt lên xe chưa
void setup() {
  pinMode(enA, OUTPUT);   //đặt trạng thái output cho các chân xuất ra l298n
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(ss[0], INPUT);   //đặt trạng thái input cho các cảm biến
  pinMode(ss[1], INPUT);
  pinMode(ss[2], INPUT);
  pinMode(ss[3], INPUT);
  pinMode(ss[4], INPUT);

  pinMode(IR, INPUT); //đặt trạng thái output cho cảm biến vật thể
  int object_sensor = 1; //khai báo kiểu dữ liệu cho cảm biến vật thể
  digitalWrite(2, HIGH);

  Serial.begin(9600);   //khởi tạo serial để đọc dữ liệu từ cảm biến xuất ra lên máy tính với tốc độ 9600
}
void Sensor_read(){
  ss[0] = digitalRead(A0);  // gán tín hiệu digital đọc từ cảm biến vào các biến ss[] từ 0 đến 5
  ss[1] = digitalRead(A1);
  ss[2] = digitalRead(A2);
  ss[3] = digitalRead(A3);
  ss[4] = digitalRead(A4);

  const int threshold = 1; // thiết lập ngưỡng cho tín hiệu lấy từ cảm bin
  //Quy ước: các cảm biến được đánh số từ 0 đến 4 từ trái sang phải, cảm biến số 2 là vị trí trung tâm, cảm biến số 0 là vị trí bên trái, cảm biến số 4 là vị trí bên phải
  if ((ss[0] < threshold) && (ss[1] < threshold) && (ss[2] < threshold) && (ss[3] < threshold) && ((ss[4] >= threshold))) {
    error = 4; // line nằm ở cảm biến số 4
  } else if ((ss[0] < threshold) && (ss[1] < threshold) && (ss[2] < threshold) && (ss[3] >= threshold) && (ss[4] >= threshold)) {
      error = 3; //line nằm ở giữa cảm biến số 3 và 4
  } else if ((ss[0] < threshold) && (ss[1] < threshold) && (ss[2] < threshold) && (ss[3] >= threshold) && (ss[4] < threshold)) {
      error = 2; //line nằm ở cảm biến số 3
  } else if ((ss[0] < threshold) && (ss[1] < threshold) && (ss[2] >= threshold) && (ss[3] >= threshold) && (ss[4] < threshold)) {
      error = 1; //line nằm giữa cảm biến số 2 và 3
  } else if ((ss[0] < threshold) && (ss[1] < threshold) && (ss[2] >= threshold) && (ss[3] < threshold) && (ss[4] < threshold)) {
      error = 0; //line nằm ở cảm biến số 2
  } else if ((ss[0] < threshold) && (ss[1] >= threshold) && (ss[2] >= threshold) && (ss[3] < threshold) && (ss[4] < threshold)) {
      error = -1; //line nằm ở giữa cảm biến số 2 và 1
  } else if ((ss[0] < threshold) && (ss[1] >= threshold) && (ss[2] < threshold) && (ss[3] < threshold) && (ss[4] < threshold)) {
      error = -2; //line nằm ở cảm biến số 1
  } else if ((ss[0] >= threshold) && (ss[1] >= threshold) && (ss[2] < threshold) && (ss[3] < threshold) && (ss[4] < threshold)) {
      error = -3; //line nằm ở giữa cảm biến số 1 và 0
  } else if ((ss[0] >= threshold) && (ss[1] < threshold) && (ss[2] < threshold) && (ss[3] < threshold) && (ss[4] < threshold)) {
      error = -4; //line nằm ở cảm biến số 0
  } else if ((ss[0] >= threshold) && (ss[1] >= threshold) && (ss[2] >= threshold) && (ss[3] < threshold) && (ss[4] < threshold)) {
      left_state = 1; //gặp góc 90 độ nằm bên trái, đặt biến trạng thái bên trái là 1
      Serial.println("góc 90 bên trái");
  }
    else if ((ss[0] < threshold) && (ss[1] < threshold) && (ss[2] >= threshold) && (ss[3] >= threshold) && (ss[4] >= threshold)) {
      right_state = 1; //gặp góc 90 độ nằm bên phải, đặt biến trạng thái bên phải là 1
      Serial.println("góc 90 bên phải");
  }
    else if ((ss[0] >= threshold) && (ss[1] >= threshold) && (ss[2] >= threshold) && (ss[3] >= threshold) && (ss[4] >= threshold)) {
      stop_state = 1; //xe chạy đến ô đích màu đen, đặt trạng thái dừng lại cho xe là 1
  }
  // Serial.print("Sai số: ");
  // Serial.println(error);
}

void PID_controller(){
  P = error; //xác định sai số
  I = constrain(I + error, -5, 5);  //giới hạn I để giảm dao động tín hiệu khi bị vọt lố, ổn định hệ thống nhanh hơn
  D = error - pre_error; //so sánh sai số hiện tại với sai số ở trạng thái lúc trước, tính toán độ vọt lố, lệch khỏi line nhiều hay ít để điều chỉnh xung pwm cho động cơ 
  PID = (kp*P) + (ki*I) + (kd*D); //tính toán PID, điều chỉnh xung băm cho động cơ quay
  Serial.print("PID: ");
  Serial.println(PID);
}

void Motor_controll(){
  // Nếu cảm biến đọc được line đang nằm bên trái và góc 90 độ, động cơ bên phải được băm xung cao hơn bên trái để quay xe về bên trái với thời gian nhất định
  while (left_state == 1){
    analogWrite(enA, 0);
    analogWrite(enB, 255);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    delay(1500);
    left_state = 0;
    break;
  }
  // Nếu cảm biến đọc được line đang nằm bên phải và góc 90 độ, động cơ bên trái được băm xung cao hơn bên phải để quay xe về bên phải với thời gian nhất định
  while (right_state == 1){
    analogWrite(enA, 255);
    analogWrite(enB, 0);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    delay(1500);
    right_state = 0;
    break;
  }
  // Nếu cảm biến đọc được đang ở ô màu đen thì cho xe dừng lại
  while (stop_state == 1){
    analogWrite(enA, 0);
    analogWrite(enB, 0);
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
  }
  // Nếu không có trường hợp đặc biệt nào, động cơ sẽ quay theo tính toán PID và bám theo line
  //int speed_difference = initial_speed - abs(PID); //tính toán tốc độ dựa trên thuật toán PID
  // int right_speed = constrain(speed_difference - PID, 0, 255); //ghi tốc độ đã được tính toán vào tốc độ động cơ bên phải
  // int left_speed = constrain(speed_difference + PID, 0, 255); //ghi tốc độ đã được tính toán vào tốc độ động cơ bên trái
  // int right_speed = initial_speed - PID;
  // int left_speed = initial_speed + PID;
  int left_speed = map(initial_speed + PID, 0, 480, 0, 225);
  int right_speed = map(initial_speed - PID, 0, 480, 0, 225);   //em chỉnh băm xung 2 hàng này, hiện tại băm xung ko được, 
  analogWrite(enB, abs(left_speed));
  analogWrite(enA, abs(right_speed));
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  Serial.print("Tốc độ bên phải và bên trái: ");
  Serial.print(left_speed);
  Serial.print("\t");
  Serial.println(right_speed);
}

void Object_detect(){
  //hàm nhận diện vật thể
  int object_sensor = digitalRead(IR);
  if ( object_sensor == 1){
    // analogWrite(enA, 255);
    // analogWrite(enB, 255);
    // digitalWrite(in1, HIGH);
    // digitalWrite(in2, LOW);
    // digitalWrite(in3, LOW);
    // digitalWrite(in4, HIGH);
    delay(500);
    redflag = 1; //gắn cờ bằng 1 để khởi động các hàm dò line
  }
}

void loop(){
  Object_detect(); //gọi hàm nhận diện vật thể
  while (redflag == 1)
  {  
    Sensor_read(); //gọi hàm đọc cảm biến
    PID_controller(); //gọi hàm tính toán PID
    Motor_controll(); //gọi hàm điều khiển động cơ
  }
}

