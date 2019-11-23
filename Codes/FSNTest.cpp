//实验组件：火焰传感器一个

//接线要求：（按需求设置）
//接三根线AO,GND,VCC
//AO接IO14

//正常输出：靠近火焰DOLED变亮，反之熄灭

void setup(){
  Serial.begin(9600);
}

void loop(){
  int sensorValue = analogRead(14);
  float voltage = sensorValue * (5/1023);

  Serial.println(voltage);
  delay(100);
}
