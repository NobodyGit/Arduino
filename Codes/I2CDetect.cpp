//实验组件：16x2 I2C LCD显示模块一个

//正常输出：串口监视器9600波特率输出扫描结果，检测到I2C在xx(eg.0x27)位置

#include <Wire.h>
#include <i2cdetect.h>

void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial.println("i2cdetect example\n");
  Serial.print("Scanning address range 0x03-0x77\n\n");
}

void loop() {
  i2cdetect();  // default range from 0x03 to 0x77
  delay(2000);
}