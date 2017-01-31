void setup() {
  pinMode(7, INPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
}

void loop() {
  if(digitalRead(7)==1){
    digitalWrite(1, HIGH);
    digitalWrite(2, LOW);
  } else {
    digitalWrite(1, LOW);
    digitalWrite(2, HIGH);
  }

}
