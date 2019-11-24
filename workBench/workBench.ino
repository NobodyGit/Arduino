#include "DHT.h"
#include <LiquidCrystal_I2C.h>

int lcdColumns = 16;
int lcdRows = 2;

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

void scrollText(int row, String message, int delayTime, int lcdColumns) {
  for (int i=0; i < lcdColumns; i++) {
    message = " " + message;  
  } 
  message = message + " "; 
  for (int pos = 0; pos < message.length(); pos++) {
    lcd.setCursor(0, row);
    lcd.print(message.substring(pos, pos + lcdColumns));
    delay(delayTime);
  }
}

void setup() {
  dht.begin();
  lcd.init();                 
  lcd.backlight();
}

void loop() {
  delay(2000);

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) {
    lcd.println(F("Failed to read from DHT sensor!"));
    return;
  }

  lcd.setCursor(0, 0);
  lcd.print("Humidity: ");
  lcd.print(h);
  lcd.print("%");
  lcd.setCursor(0, 1);
  char temp[30];
  sprintf(temp, "Temperature: %f°C %f°F", t, f);
  scrollText(1,temp,500,lcdColumns);
}
