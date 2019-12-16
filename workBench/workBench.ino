#include "DHT.h"
#include <task.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <analogWrite.h>
#include <BlynkSimpleEsp32.h>
#include <LiquidCrystal_I2C.h>

void Task_Read   ( void *pvParameters );
void Task_Serial ( void *pvParameters );
void Task_LCD    ( void *pvParameters );
void Task_Buzzer1( void *pvParameters );

//blynk and wifi related variables
#define BLYNK_PRINT Serial
char auth[] = "hfJ0lheL1MRy3lcTOdKXW_cCAf0EmCQd"; //blynk project token
char ssid[] = "diya-laptop"; //wifi id
char pass[] = "a1MckxYh"; //wifi password

//sensors' pins
const int smoke_sensor_pin = 2;
const int fire_sensor_pin = 13;
const int light_sensor_pin = 14;

//Init DHT sensor
#define DHTPIN 4
#define DHTTYPE DHT22 //DHT type
DHT dht(DHTPIN, DHTTYPE); // DHT init

//sensors detection standard
int smoke_standard = 300;

//actuators' pins
const int relay1_pin = 16; //relay1 pin, control ...
const int led1_pin = 18; //led1 pin, actuator of light sensor
const int buzzer1_pin = 27; //buzzer1 pin, actuator of smoke sensor

//sensors' values
int light_sensor_value, fire_sensor_value, smoke_sensor_value;
float humi, temp, fah_temp, heatind, fah_heatind; //dht values: humidity, temperature, temperature based on Fahrenheit, heat index, heat index based on Fahrenheit

//Init LCD
const int lcdColumns = 16, lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

//LED PWM configuration
const int ledChannel = 0;
const int freq = 5000;
const int resolution = 8;

BLYNK_WRITE(V0){
  smoke_standard = param.asInt(); // assigning incoming value from pin V0 to a variable
  Serial.print("Now smoke detection standard is: ");
  Serial.println(smoke_standard);
}

void setup() {
  Serial.begin(9600);

  //Connectting to wifi and blynk
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);
  int wifi_ctr = 0;
  while(WiFi.status() != WL_CONNECTED) {
    vTaskDelay( 500 / portTICK_PERIOD_MS ); //retry every 500ms
    Serial.println("Connection failed, retrying...");
  }
  Serial.println("WiFi connected");

  Blynk.begin(auth, ssid, pass);

  //input pins
  pinMode(smoke_sensor_pin, INPUT);
  pinMode(fire_sensor_pin, INPUT);
  pinMode(light_sensor_pin, INPUT);

  //light to light sensor
  ledcSetup(ledChannel, freq, resolution);// configure LED PWM functionalitites
  ledcAttachPin(led1_pin, ledChannel);// attach the channel to the GPIO to be controlled

  //DHT setup
  dht.begin();

  //output pins
  pinMode(relay1_pin, OUTPUT);
  pinMode(buzzer1_pin, OUTPUT);
  
  //create multiple tasks
  xTaskCreate(Task_Read, "Task Read", 10000, NULL, 2, NULL); //Read data from sensors
  xTaskCreate(Task_Serial, "Task Serial", 10000, NULL, 3, NULL); //Display data to Serial
  xTaskCreate(Task_LCD, "Task LCD", 10000, NULL, 3, NULL); //Display data to LCD
  xTaskCreate(Task_Buzzer1, "Task Buzzer1", 10000, NULL, 3, NULL); //Control buzzer1
}

void loop() {
  // // ledcWrite(ledChannel,400-light_sensor_value); //Control led1 by PWM

  Blynk.run();
}

void Task_Read (void *pvParameters ){
  while(1){
    //read from normal sensors
    smoke_sensor_value = analogRead(smoke_sensor_pin);
    fire_sensor_value = analogRead(fire_sensor_pin);
    light_sensor_value = analogRead(light_sensor_pin);

    //read from dht sensor
    humi = dht.readHumidity(); //Read humidity
    temp = dht.readTemperature(); // Read temperature as Celsius (the default)
    fah_temp = dht.readTemperature(true); // Read temperature as Fahrenheit (isFahrenheit = true)
    heatind = dht.computeHeatIndex(fah_temp, humi); // Compute heat index in Fahrenheit (the default)
    fah_heatind = dht.computeHeatIndex(temp, humi, false); // Compute heat index in Celsius (isFahreheit = false)

    vTaskDelay( 500 / portTICK_PERIOD_MS ); //read every 500ms
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
  char sensor_value[6];
  while(1){
    if (isnan(humi)){
      lcd.setCursor(0, 0);
      lcd.print("Fail to read");
      lcd.setCursor(0, 1);
      lcd.print("       humidity.");
      vTaskDelay( 1000 / portTICK_PERIOD_MS );
    }
    else{
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("DHT sensor:");
      dtostrf(humi, 2, 1, sensor_value);
      text = "Humi: " + String(sensor_value) + "%";
      lcd.setCursor(0, 1);
      lcd.print(text);
      vTaskDelay( 3000 / portTICK_PERIOD_MS );
    }

    if (isnan(temp)){
      lcd.setCursor(0, 0);
      lcd.print("Fail to read");
      lcd.setCursor(0, 1);
      lcd.print("    temperature.");
      vTaskDelay( 1000 / portTICK_PERIOD_MS );
    }
    else{
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("DHT sensor:");
      dtostrf(temp, 2, 1, sensor_value);
      text = "Temp(\337C): " + String(sensor_value);
      lcd.setCursor(0, 1);
      lcd.print(text);
      vTaskDelay( 3000 / portTICK_PERIOD_MS );
    }

    if (isnan(fah_temp)){
      lcd.setCursor(0, 0);
      lcd.print("Fail to read");
      lcd.setCursor(0, 1);
      lcd.print("       fah_temp.");
      vTaskDelay( 1000 / portTICK_PERIOD_MS );
    }
    else{
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("DHT sensor:");
      dtostrf(fah_temp, 2, 1, sensor_value);
      text = "Temp(\337F): " + String(sensor_value);
      lcd.setCursor(0, 1);
      lcd.print(text);
      vTaskDelay( 3000 / portTICK_PERIOD_MS );
    }
  }
}

void Task_Buzzer1( void *pvParameters ){
  while(1){
    if(smoke_sensor_value>300){
      unsigned char i,j;

      for(i=0;i<80;i++)//输出一个频率的s声音
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
  }
}
