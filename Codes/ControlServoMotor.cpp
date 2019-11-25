#define BLYNK_PRINT Serial

#include <WiFi.h>
//实验组件：Blynk Slider，伺服电机一个

//组件描述(Servo Motor)：
//红线接VCC(mid)，咖啡色（黑色）接GND，橙色（黄或白）接信号
//注意：接错可能导致组件烧毁！
//组件描述（Blynk Slider）：
//Blynk app虚拟组件
//Slider PIN设置为V1，最大值设为180，其余默认

//接线要求：（按需求设置）
//橙线接IO16

//正常输出：伺服电机根据Slider回传值转动，串口监视器115200波特率输出Slider回传值

#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

const int thisPin = 16;
#include <ESP32Servo.h>
Servo myservo;

char auth[] = "hfJ0lheL1MRy3lcTOdKXW_cCAf0EmCQd";
//Blynk 项目token

char ssid[] = "MYHL";
char pass[] = "123456789";
//WiFi信息：
//ssid为WiFi名，pass为密码

BLYNK_WRITE(V1)
{
	int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
	// You can also use:
	// String i = param.asStr();
	// double d = param.asDouble();
	Serial.print("V1 Slider value is: ");
	Serial.println(pinValue);
	myservo.write(pinValue);
}

void setup()
{
	Serial.begin(115200);
	myservo.attach(thisPin);

	delay(10);
	Serial.print("Connecting to ");
	Serial.println(ssid);

	WiFi.begin(ssid, pass);
	int wifi_ctr = 0;
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}

	Serial.println("WiFi connected");

	Blynk.begin(auth, ssid, pass);
}

void loop()
{
	Blynk.run();
}
