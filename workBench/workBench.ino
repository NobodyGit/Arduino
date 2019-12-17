#include "DHT.h"
#include <task.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <analogWrite.h>
#include <BlynkSimpleEsp32.h>
#include <LiquidCrystal_I2C.h>

void Task_Read     ( void *pvParameters );
void Task_Serial   ( void *pvParameters );
void Task_LCD      ( void *pvParameters );
void Task_Buzzer1  ( void *pvParameters );
void Task_Relay    ( void *pvParameters );

void   displayText ( String row1Text, String row2Text, int delayTime, int scrollDuration );
String appendSpace ( String text );

//blynk and wifi related variables
#define BLYNK_PRINT Serial
char auth[] = "hfJ0lheL1MRy3lcTOdKXW_cCAf0EmCQd"; //blynk project token
char ssid[] = "diya-laptop"; //wifi id
char pass[] = "a1MckxYh"; //wifi password

//sensors' pins
#define MQ2PIN 39  //smoke sensor pin
#define LSNPIN 14 //light sensor pin
#define FSNPIN 12 //fire sensor pin
#define DHTPIN 13  //DHT sensor pin

//actuators' pins
const int relay_pin1 = 27; //relay1 pin, control water pump
const int relay_pin2 = 16; //relay2 pin, control fan
const int buzzer1_pin = 17; //buzzer1 pin, actuator of smoke sensor
const int led1_pin = 18; //led1 pin, actuator of light sensor

//Init DHT sensor
#define DHTTYPE DHT22 //DHT type
DHT dht(DHTPIN, DHTTYPE); // DHT init

//sensors standard
int sensors_detect_duration = 500;
int smoke_alarm_level = 1500;
float fan_switch_temp = 26;

//sensors' values
int light_sensor_value = 0, fire_sensor_value = 0;
float smoke_sensor_value = 0;
float humi = 0, temp = 0, fah_temp = 0, heatind = 0, fah_heatind = 0; //dht values: humidity, temperature, temperature based on Fahrenheit, heat index, heat index based on Fahrenheit

//Init LCD
const int lcdColumns = 16, lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

//LED PWM configuration
const int ledChannel = 0;
const int freq = 5000;
const int resolution = 8;

//flags
bool wifi_connected = false;

BLYNK_READ(V0){
  Blynk.virtualWrite(0, smoke_sensor_value);
}

BLYNK_READ(V1){
  Blynk.virtualWrite(1, temp);
}

BLYNK_READ(V2){
  Blynk.virtualWrite(2, humi);
}

BLYNK_WRITE(V3){
  sensors_detect_duration = param.asInt() * 500;
  Serial.print("Now sensors' detect duration is: ");
  Serial.println(sensors_detect_duration);
}

BLYNK_WRITE(V4){
  fan_switch_temp = param.asFloat();
  Serial.print("Now temperature switch standard is: ");
  Serial.println(fan_switch_temp);
}

void setup() {
  Serial.begin(9600);

  //input pins
  pinMode(MQ2PIN, INPUT);
  pinMode(LSNPIN, INPUT);
  pinMode(FSNPIN, INPUT);

  //light to light sensor
  ledcSetup(ledChannel, freq, resolution);// configure LED PWM functionalitites
  ledcAttachPin(led1_pin, ledChannel);// attach the channel to the GPIO to be controlled

  //DHT setup
  dht.begin();

  //output pins
  pinMode(relay_pin1, OUTPUT);
  pinMode(relay_pin2, OUTPUT);
  pinMode(buzzer1_pin, OUTPUT);

  //Connectting to wifi and blynk
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);
  int wifi_ctr = 0, count = 0;
  while(WiFi.status() != WL_CONNECTED) {
    delay( 500 ); //retry every 500ms
    Serial.println("Connection failed, retrying...");
    if(++count>9){
      Serial.println("Fail to connect after retrying 10 times. Please check your wifi configuration.");
      Serial.println("Initializing system offline...");
      break;
    }
  }
  if(count<10){
    wifi_connected = true;
    Serial.println("WiFi connected");
    Blynk.begin(auth, ssid, pass);
  }
  
  //create multiple tasks
  xTaskCreate(Task_Read, "Task Read", 10000, NULL, 1, NULL); //Read data from sensors
  xTaskCreate(Task_Serial, "Task Serial", 10000, NULL, 3, NULL); //Display data to Serial
  xTaskCreate(Task_LCD, "Task LCD", 10000, NULL, 3, NULL); //Display data to LCD
  xTaskCreate(Task_Buzzer1, "Task Buzzer1", 10000, NULL, 2, NULL); //Control buzzer1
  xTaskCreate(Task_Relay, "Task Relay", 10000, NULL, 2, NULL); //Control Relay

  Serial.println("System initialized.");
}

void loop() {
  // // ledcWrite(ledChannel,400-light_sensor_value); //Control led1 by PWM

  if(wifi_connected)
    Blynk.run();
}

void Task_Read (void *pvParameters ){
  while(1){
    //read from normal sensors
    smoke_sensor_value = analogRead(MQ2PIN);
    light_sensor_value = analogRead(LSNPIN);
    fire_sensor_value = analogRead(FSNPIN);

    //read from dht sensor
    humi = dht.readHumidity(); //Read humidity
    temp = dht.readTemperature(); // Read temperature as Celsius (the default)
    fah_temp = dht.readTemperature(true); // Read temperature as Fahrenheit (isFahrenheit = true)
    heatind = dht.computeHeatIndex(fah_temp, humi); // Compute heat index in Fahrenheit (the default)
    fah_heatind = dht.computeHeatIndex(temp, humi, false); // Compute heat index in Celsius (isFahreheit = false)

    vTaskDelay( sensors_detect_duration / portTICK_PERIOD_MS ); //read every 500ms
  }
}

void Task_Serial( void *pvParameters ){
  while(1){
    Serial.println("-------------------------------------------------------------------------");

    Serial.print("Light sensor value: ");
    Serial.print(light_sensor_value);
    Serial.print(";  Fire sensor value: ");
    Serial.print(fire_sensor_value);
    Serial.print(";  Smoke sensor value: ");
    Serial.println(smoke_sensor_value);

    if(smoke_sensor_value>smoke_alarm_level){
      Serial.println("Smoke concentration is in dangerous level now.");
    }
    
    // Check if any reads failed and exit early (to try again).
    if (isnan(humi) || isnan(temp) || isnan(fah_temp)){
      Serial.print(F("Error message: Failed to read from DHT sensor. Please check DHT sensor, dht sensor pin: "));
      Serial.println(DHTPIN);
    }
    else{
      Serial.print(F("Humidity: "));
      Serial.print(humi);
      Serial.print(F("%;  Temperature: "));
      Serial.print(temp);
      Serial.print(F("°C/"));
      Serial.print(fah_temp);
      Serial.print(F("°F;  Heat index: "));
      Serial.print(heatind);
      Serial.print(F("°C/"));
      Serial.print(fah_heatind);
      Serial.println(F("°F"));
    }
    
    Serial.println("-------------------------------------------------------------------------");

    vTaskDelay( 1000 / portTICK_PERIOD_MS ); //display every second
  }
}

void Task_LCD( void *pvParameters ){
  lcd.init(); //screen display
  lcd.backlight(); // turn on LCD backlight

  String text;
  char sensor_value[20];
  while(1){
    if (isnan(humi)){
      displayStaticText("Fail to read", "      humidity", 2000);
    }
    else{
      dtostrf(humi, 2, 1, sensor_value);
      text = "       " + String(sensor_value) + "%";
      displayText("Now Humidity:", text, 3000, 200);
    }

    if (isnan(temp)){
      displayStaticText("Fail to read", "   temperature", 2000);
    }
    else{
      dtostrf(temp, 2, 1, sensor_value);
      text = "        " + String(sensor_value);
      displayText("Now Temp(\337C):", text, 3000, 200);
    }

    if (isnan(fah_temp)){
      displayStaticText("Fail to read", "      fah_temp", 2000);
    }
    else{
      dtostrf(fah_temp, 2, 1, sensor_value);
      text = "        " + String(sensor_value);
      displayText("Now Temp(\337F):", text, 3000, 200);
    }

    if(light_sensor_value==0 || light_sensor_value==4095){
      displayStaticText("Fail to read", "         light", 2000);
    }
    else{
      text = "        " + String(light_sensor_value);
      displayText("Current light:", text, 3000, 200);
    }

    if(smoke_sensor_value==0 || smoke_sensor_value==4095){
      displayStaticText("Fail to read", "         smoke", 2000);
    }
    else{
      while(smoke_sensor_value>smoke_alarm_level){
        displayStaticText("Warnning!", "Fire detected!", sensors_detect_duration);
      }
      dtostrf(smoke_sensor_value, 2, 1, sensor_value);
      text = "        " + String(sensor_value);
      displayText("Current smoke:", text, 3000, 200);
    }
  }
}

void displayText(String row1Text, String row2Text, int delayTime, int scrollDuration){
  //Row1 enters
  String tempStr = appendSpace(row1Text);
  for(int pos=0; pos<16; pos++){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(tempStr.substring(pos));
    vTaskDelay( scrollDuration / portTICK_PERIOD_MS );
  }

  //Row2 enters
  tempStr = appendSpace(row2Text);
  for(int pos=0; pos<16; pos++){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(row1Text);
    lcd.setCursor(0, 1);
    lcd.print(tempStr.substring(pos));
    vTaskDelay( scrollDuration / portTICK_PERIOD_MS );
  }

  //Static text displays
  displayStaticText(row1Text, row2Text, 2000);

  //Row2 exits
  for(int pos=0; pos<row2Text.length(); pos++){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(row1Text);
    lcd.setCursor(0, 1);
    lcd.print(row2Text.substring(pos));
    vTaskDelay( scrollDuration / portTICK_PERIOD_MS );
  }

  //Row1 exits
  for(int pos=0; pos<row1Text.length(); pos++){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(row1Text.substring(pos));
    vTaskDelay( scrollDuration / portTICK_PERIOD_MS );
  }
}

void displayStaticText(String row1Text, String row2Text, int delayTime){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(row1Text);
  lcd.setCursor(0, 1);
  lcd.print(row2Text);
  vTaskDelay( delayTime / portTICK_PERIOD_MS );
}

String appendSpace(String text){
  for(int i=0; i<lcdColumns; i++){
    text = " " + text;
  }
  return text;
}

void Task_Buzzer1( void *pvParameters ){
  while(1){
    if(smoke_sensor_value>smoke_alarm_level){
      unsigned char i,j;

      for(i=0;i<50;i++)//输出一个频率的s声音
      {
        digitalWrite(buzzer1_pin,HIGH);//发声音
        vTaskDelay( 1 / portTICK_PERIOD_MS );//延时1ms
        digitalWrite(buzzer1_pin,LOW);//不发声音
        vTaskDelay( 1 / portTICK_PERIOD_MS );//延时ms
      }
      for(i=0;i<100;i++)//输出另一个频率的声音,这里的100与前面的80一样，用来控制频率，可以自己调节
      {
        digitalWrite(buzzer1_pin,HIGH);
        vTaskDelay( 2 / portTICK_PERIOD_MS );
        digitalWrite(buzzer1_pin,LOW);
        vTaskDelay( 2 / portTICK_PERIOD_MS );
      }
    }
    else{
      vTaskDelay( sensors_detect_duration / portTICK_PERIOD_MS );//延时1s
    }
  }
}

void Task_Relay( void *pvParameters ){
  while(1){
    if(temp>fan_switch_temp){
      digitalWrite(relay_pin2, HIGH);
    }
    else{
      digitalWrite(relay_pin2, LOW);
    }

    vTaskDelay( sensors_detect_duration / portTICK_PERIOD_MS );
  }
}
