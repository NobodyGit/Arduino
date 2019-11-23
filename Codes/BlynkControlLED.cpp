//实验组件：Blynk Button，ESP32 开发板，LED小灯（可选）

//组件描述（Blynk Button）：
//Blynk app虚拟组件
//Button PIN设置为gp2，Mode设置为Switch，其余默认

//接线要求：（可选）
//LED小灯长端接IO2，短端接GND

//正常输出：通过开关Blynk Button来控制开发板信号灯和LED小灯状态，开为亮，关为暗
//输出示例：
//[46191] Connecting to diya-laptop
//[46239] Connected to WiFi
//[46239] IP: 192.168.137.127
//[46240] 
//    ___  __          __
//   / _ )/ /_ _____  / /__
//  / _  / / // / _ \/  '_/
// /____/_/\_, /_//_/_/\_\
//        /___/ v0.6.1 on ESP32
//
//[46246] Connecting to blynk-cloud.com:80
//[51247] Connecting to blynk-cloud.com:80
//[51785] Ready (ping: 14ms).


#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = "hfJ0lheL1MRy3lcTOdKXW_cCAf0EmCQd";
//Blynk 项目token

char ssid[] = "diya-laptop";
char pass[] = "a123456Z";
//WiFi信息：
//ssid为WiFi名，pass为密码

void setup(){
  Serial.begin(115200);

  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);
  int wifi_ctr = 0;
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");

  Blynk.begin(auth, ssid, pass);
}

void loop(){
  Blynk.run();
}
 