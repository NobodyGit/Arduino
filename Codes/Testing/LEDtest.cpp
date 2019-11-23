int red_light_pin= 17;
int green_light_pin = 25;
int blue_light_pin = 26;
void setup() {
  //setup channel 0 with frequency 312500 Hz
  sigmaDeltaSetup(0, 312500);
  //attach pin 18 to channel 0
  sigmaDeltaAttachPin(red_light_pin,0);
  
  //setup channel 1 with frequency 312500 Hz
  sigmaDeltaSetup(1, 312500);
  //attach pin 18 to channel 1
  sigmaDeltaAttachPin(green_light_pin,1);
  
  //setup channel 2 with frequency 312500 Hz
  sigmaDeltaSetup(2, 312500);
  //attach pin 18 to channel 2
  sigmaDeltaAttachPin(blue_light_pin,2);
}
void loop() {
  RGB_color(255, 0, 0); // Red
  delay(1000);
  RGB_color(0, 255, 0); // Green
  delay(1000);
  RGB_color(0, 0, 255); // Blue
  delay(1000);
  RGB_color(255, 255, 125); // Raspberry
  delay(1000);
  RGB_color(0, 255, 255); // Cyan
  delay(1000);
  RGB_color(255, 0, 255); // Magenta
  delay(1000);
  RGB_color(255, 255, 0); // Yellow
  delay(1000);
  RGB_color(255, 255, 255); // White
  delay(1000);
}
void RGB_color(int red_light_value, int green_light_value, int blue_light_value)
 {
  sigmaDeltaWrite(red_light_pin, red_light_value);
  sigmaDeltaWrite(green_light_pin, green_light_value);
  sigmaDeltaWrite(blue_light_pin, blue_light_value);
}