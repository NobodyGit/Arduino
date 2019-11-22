int speakerPIN = 4;

void setup(){
  pinMode(speakerPIN, OUTPUT);
}

void loop(){
  digitalWrite(speakerPIN, HIGH);
  delay(500);
  digitalWrite(speakerPIN, LOW);
  delay(500);
}
