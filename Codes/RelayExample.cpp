//实验组件：一路光耦隔离继电器驱动模块一个，LED小灯一个

//组件描述（继电器模块）：
//输入端：
//DC+接VCC端，DC-接GND端，IN接信号端，均接控制电路
//输出端：
//COM为公共端，NC为常闭端，NO为常开端，均接被控电路

//接线要求：（按需求设置）
//继电器输入端DC+接5V，DC-接GND，IN接IO2
//LED小灯长端接被控电路电源，短端接继电器输出端NO/NC（两者皆可）
//继电器输出端COM接被控电路地线（GND）

//正常输出：继电器模块电源灯常亮，开发板信号灯、继电器模块信号灯、LED小灯同步变化（每隔5秒亮5秒）

int pinRelay = 2; //管脚D3连接到继电器模块的信号脚
 
void setup() {
  pinMode(pinRelay, OUTPUT); //设置pinRelay脚为输出状态
}
 
void loop() { 
  digitalWrite(pinRelay, HIGH); //设置pinRelay脚为高电平状态
//   digitalWrite(pinRelay, HIGH);//输出HIGH电平,继电器模块闭合
   delay(5000); //等待5000毫秒

  digitalWrite(pinRelay, LOW); //设置pinRelay脚为高电平状态
//   digitalWrite(pinRelay, LOW);//输出LOW电平,继电器模块断开
   delay(5000); //等待5000毫秒
}