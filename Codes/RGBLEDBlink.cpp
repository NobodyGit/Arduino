//实验组件：4pin common anode RGB LED小灯一个

//组件描述(RGB LED)：
//4pin common anode RGB LED 最长端为VCC
//VCC端在左侧，LED接口从左至右依次为R,VCC,G,B

//接线要求：（按需求设置）
//R接17
//G接25
//B接26

//正常输出：RGB LED 依次闪烁红绿蓝光

int red= 17;
int green = 25;
int blue = 26;

void setup()
{
// initialize the digital pin as an output.
pinMode(red, OUTPUT);
pinMode(green, OUTPUT);
pinMode(blue, OUTPUT);
digitalWrite(red, HIGH);
digitalWrite(green, HIGH);
digitalWrite(blue, HIGH);
}
 
// the loop routine runs over and over again forever:
void loop() {
digitalWrite(red, LOW); // turn the LED on 
delay(1000); // wait for a second
digitalWrite(red, HIGH); // turn the LED off by making the voltage LOW
delay(1000); // wait for a second
digitalWrite(green, LOW); // turn the LED on 
delay(1000); // wait for a second
digitalWrite(green, HIGH); // turn the LED off by making the voltage LOW
delay(1000); // wait for a second
digitalWrite(blue, LOW); // turn the LED on
delay(1000); // wait for a second
digitalWrite(blue, HIGH); // turn the LED off by making the voltage LOW
delay(1000); // wait for a second
}