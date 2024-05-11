int ss1 = A0; 
int ss2= A1;
int ss3 = A2;
int ss4 = A3;
int ss5 = A4;
void setup() {
  pinMode(ss1, INPUT);
  pinMode(ss2, INPUT);
  pinMode(ss3, INPUT);
  pinMode(ss4, INPUT);
  pinMode(ss5, INPUT);
  Serial.begin(9600);
}
void loop() {
  // đọc tín hiệu analog từu 5 cảm biến
  int ssn1 = analogRead(ss1);
  int ssn2 = analogRead(ss2);
  int ssn3 = analogRead(ss3);
  int ssn4 = analogRead(ss4);
  int ssn5 = analogRead(ss5);
  //in ra serial giá trị analg của 5 cảm biến
  Serial.print(ssn1);
  Serial.print("\t");
  Serial.print(ssn2);
  Serial.print("\t");
  Serial.print(ssn3);
  Serial.print("\t");
  Serial.print(ssn4);
  Serial.print("\t");
  Serial.println(ssn5);

}
