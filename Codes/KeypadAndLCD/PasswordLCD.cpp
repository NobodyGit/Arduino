//实验组件：4x4 Keypad一个，16x2 I2C LCD显示模块一个

//组件描述（4x4 Keypad）：
//8个接口，横竖电路各4个，左4为行，右4为列，全部接信号
//组件描述（16x2 I2C LCD显示模块）：
//4个接口，按模块接口编号默认接线

//接线要求：
//Keypad正面朝上，从左至右依次接18，19，23，5，13，12，14，27
//I2C LCD显示模块按编号默认接线

//正常输出：输入7位密码，密码正确输出"Correct"，错误输出"Incorrect",之后返回初始界面

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

#define Password_Length 8 

char Data[Password_Length]; 
char Master[Password_Length] = "123A456"; 
byte data_count = 0, master_count = 0;
bool Pass_is_good;
char customKey;

const byte ROWS = 4; 
const byte COLS = 4; 

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {18, 19, 23, 5}; 
byte colPins[COLS] = {13, 12, 14, 27}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

LiquidCrystal_I2C lcd(0x27, 16, 2);
//默认0x27，0x27是根据I2CDetect所得I2C模块位置编号
//16，2表示LCD显示模式为16x2

void setup(){
  Serial.begin(9600);
  lcd.backlight();
  lcd.init(); 
}

void loop(){
  lcd.setCursor(0,0);
  lcd.print("Enter Password:");

  customKey = customKeypad.getKey();
  if (customKey){
    Data[data_count] = customKey; 
    lcd.setCursor(data_count,1); 
    lcd.print(Data[data_count]); 
    data_count++; 
    }

  if(data_count == Password_Length-1){
    lcd.clear();

    if(!strcmp(Data, Master)){
      lcd.print("Correct");
      delay(1000);
      }
    else{
      lcd.print("Incorrect");
      delay(1000);
      }
    
    lcd.clear();
    clearData();  
  }
}

void clearData(){
  while(data_count !=0){
    Data[data_count--] = 0; 
  }
  return;
}
