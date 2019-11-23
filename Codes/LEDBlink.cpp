//实验组件：LED小灯一个

//接线要求：（按需求设置）
//LED小灯长端接IO18，短端接GND

//正常输出：小灯间断闪烁

int ledpin = 18;

void setup() {
  pinMode(ledpin, OUTPUT); //justando o IO25 como saída
}
 
void loop() {
  digitalWrite(ledpin, HIGH); //alterando o nivel do pino para ALTO
  delay(1000);            //aguarda 1000ms(um segundo)
  digitalWrite(ledpin, LOW);  //alterndo o nível do pino para BAIXO
  delay(1000);            //aguarda 1000ms(um segundo)
}