#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
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

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup(){
  Serial.begin(9600);
  lcd.backlight();
  lcd.init(); 
}

void loop(){
  char customKey = customKeypad.getKey();
  if (customKey){
    Serial.println(customKey);
    lcd.clear();
    lcd.setCursor(0, 0); 
    lcd.print(customKey);
  }
}
