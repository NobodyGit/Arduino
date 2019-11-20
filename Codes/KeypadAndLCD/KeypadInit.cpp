//实验组件：4x4 Keypad一个

//组件描述（4x4 Keypad）：
//8个接口，横竖电路各4个，左4为行，右4为列，全部接信号

//接线要求：（按需求设置）
//Keypad正面朝上，从左至右依次接18，19，23，5，13，12，14，27

//正常输出：串口监视器9600波特率按Keypad键入进行输出

#include <Keypad.h>

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

void setup(){
  Serial.begin(9600);
}
  
void loop(){
  char customKey = customKeypad.getKey();
  
  if (customKey){
    Serial.println(customKey);
  }
}