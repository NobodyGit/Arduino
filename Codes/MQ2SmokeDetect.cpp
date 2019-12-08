//实验组件：MQ2 烟雾传感器一个

//组件描述（MQ2 烟雾传感器）：
//AO接口为Analog out，对应Analog pin，DO接口为Digital out，对应Digital pin

//接线要求：（按需求设置）
//烟雾传感器VCC接5v，GND接GND，AO接analog pin 2，DO可选择

#define MQ2pin (2)

float sensorValue;  //variable to store sensor value

void setup()
{
  Serial.begin(9600); // sets the serial port to 9600
  Serial.println("Gas sensor warming up!");
  delay(20000); // allow the MQ-6 to warm up
}

void loop()
{
  sensorValue = analogRead(MQ2pin); // read analog input pin 2
  
  Serial.print("Sensor Value: ");
  Serial.print(sensorValue);
  
  if(sensorValue > 300)
  {
    Serial.print(" | Smoke detected!");
  }
  
  Serial.println("");
  delay(2000); // wait 2s for next reading
}