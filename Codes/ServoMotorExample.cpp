//实验组件：伺服电机一个

//组件描述(Servo Motor)：
//红线接VCC(mid)，咖啡色（黑色）接GND，橙色（黄或白）接信号
//注意：接错可能导致组件烧毁！

//接线要求：（按需求设置）
//橙线接IO16

//正常输出：伺服电机来回转动

const int thisPin = 16;
#include <ESP32Servo.h>
Servo myservo;

void setup() {
	myservo.attach(thisPin);  // attaches the servo on thisPin to the servo object
}

void loop() {
	for (int pos = 0; pos <= 180; pos++) { // goes from 0 degrees to 180 degrees
		// in steps of 1 degree
		myservo.write(pos);              // tell servo to go to position in variable 'pos'
		delay(15);                       // waits 15ms for the servo to reach the position
	}
	for (int pos = 180; pos >= 0; pos--) { // goes from 180 degrees to 0 degrees
    	myservo.write(pos);              // tell servo to go to position in variable 'pos'
    	delay(15);                       // waits 15ms for the servo to reach the position
 	}
}