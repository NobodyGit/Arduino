//实验组件：16x2 I2C LCD显示模块一个，超声波传感器一个

//接线要求：（按需求设置）
//I2C LCD显示模块按编号默认接线
//超声波传感器VCC和GND默认接线，trig端接IO12，echo端接IO13

//正常输出：显示模块按传感器检测到的距离数据进行输出（单位：cm）

#include "Ultrasonic.h"
#include <LiquidCrystal_I2C.h>

int lcdColumns = 16;
int lcdRows = 2;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  
Ultrasonic ultrasonic(12,13);

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("HC-SR4 testing..");
  delay(1000);
}

void loop()
{
  lcd.setCursor(0, 1);
  lcd.print(ultrasonic.Ranging(CM));
  lcd.print("cm  ");
  delay(100);
}
