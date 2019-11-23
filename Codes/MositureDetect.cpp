//实验组件：土壤湿度传感器模块一个

//组件描述（土壤湿度传感器模块）：
//两部分：传感器和输出模块

//接线要求：（按需求设置）
//传感器部分接输出模块（对应2PIN传感器输入）
//输出模块4PIN部分VCC和GND默认接线，AO接IO12（传递传感器数据），DO接IO13（返回开发板输出）

//正常输出：串口监视器115200波特率每隔1秒输出检测到的湿度（默认4095），传感器模块上的开关信号灯随之变化，数值越小，灯越亮

//注：上传代码时出现错误，尝试先不接AO和DO端，上传后再接

int sensorpin=12;   //setting the sensor pin
int outputpin=13;   //setting the output pin
int moisture;  //the variable to control

void setup(){
  Serial.begin(115200);         //must begin to use serial monitor
  pinMode(sensorpin,INPUT);    //setting sensor pin to be input
  pinMode(outputpin,OUTPUT);   //setting output pin to be output
}

void loop(){
  moisture=analogRead(sensorpin);  //read the value from sensor pin
  Serial.println(moisture);  //show the value receive in serial monitor
  
  
  if(moisture<=600){                //the criticall value to trigger the solenoid
    digitalWrite(outputpin,LOW);    //you can alternate it depends on yourself
  }
  else{
    digitalWrite(outputpin,HIGH);
  }
  delay(1000);
}