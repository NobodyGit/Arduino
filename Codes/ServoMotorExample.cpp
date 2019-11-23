//实验组件：伺服电机一个

//组件描述(Servo Motor)：
//红线接VCC(mid)，咖啡色（黑色）接GND，橙色（黄或白）接信号

//接线要求：（按需求设置）
//橙线接IO16

//正常输出：伺服电机间歇转动

const int thisPin = 16;
#include <ESP32Servo.h>
Servo myservo;

void setup() {
	Serial.begin(115200);
}

void loop() {
	if (!myservo.attached()) {
		myservo.setPeriodHertz(50); // standard 50 hz servo
		myservo.attach(33, 1000, 2000); // Attach the servo after it has been detatched
	}
	myservo.write(0);
	// iterate over the pins:
		if (ESP32PWM::hasPwm(thisPin) &&  // Is it possible for this pin to PWM
				(ESP32PWM::channelsRemaining() > 0 || // New channels availible to allocate
						pwmFactory(thisPin) != NULL || // already allocated this pin in the factory
						thisPin == 25 || // one of the  2 DAC outputs, no timer needed
						thisPin == 26)) { // one of the 2 DAC outputs, no timer needed
			if (pwmFactory(thisPin) == NULL) { // check if its the first time for the pin or its a DAC
				if (thisPin == 25 || // one of the 2 DAC outputs, no timer needed
						thisPin == 26) {
					Serial.println("DAC to pin " + String(thisPin));
				} else
					Serial.println("Writing to pin " + String(thisPin));
				pinMode(thisPin, OUTPUT);
			}
			// fade the LED on thisPin from off to brightest:
			for (int brightness = 0; brightness < 255; brightness++) {
				analogWrite(thisPin, brightness);
				delay(1);
				myservo.write(brightness);
			}
			// fade the LED on thisPin from brithstest to off:
			for (int brightness = 255; brightness >= 0; brightness--) {
				analogWrite(thisPin, brightness);
				myservo.write(brightness);
				delay(1);
			}

		}
	myservo.detach(); // Turn the servo off for a while
	delay(2000);

}