05P1205P1205_practice_2.ino05_practice_2.ino05_practice_2.ino05_practice_2.ino05_practice_2.inovoid setup() {
  pinMode(7, OUTPUT);
  Serial.begin(115200);
 
}

void loop() {
  digitalWrite(7, LOW);
  delay(1000);
  for(int i=0; i<5; i++){
    digitalWrite(7, HIGH);
    delay(200);
    digitalWrite(7, LOW);
    delay(200);
  }
  digitalWrite(7, HIGH);
  while(1){}
}
