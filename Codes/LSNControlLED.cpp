//实验要求：光传感器(LSN)一个，2pin LED小灯一个

//接线要求：（按需求设置）
//LSN DO端 接 IO14，其余默认接线
//2pin-LED 长端 接 IO18，短端接GND

//正常输出：对LSN进行遮光则LED小灯亮，反之熄灭

int ldrPin = 14;
int ledPin= 18;

int val = 0;
void setup() {
  pinMode(ldrPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}
 
void loop() {
  int val = digitalRead(ldrPin);
  Serial.println(val);
 
  if(val == HIGH ){
    digitalWrite(ledPin, HIGH);
    Serial.println("LED ON");
  }
  else{
    digitalWrite(ledPin, LOW);
    Serial.println("LED OFF");
  }
}