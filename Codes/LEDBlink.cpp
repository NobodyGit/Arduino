void setup() {
  pinMode(25, OUTPUT); //justando o IO25 como saída
}
 
void loop() {
  digitalWrite(25, HIGH); //alterando o nivel do pino para ALTO
  delay(1000);            //aguarda 1000ms(um segundo)
  digitalWrite(25, LOW);  //alterndo o nível do pino para BAIXO
  delay(1000);            //aguarda 1000ms(um segundo)
}