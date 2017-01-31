void setup() {
  Serial.begin(9600);
  pinMode(7, INPUT_PULLUP);
  Serial.println("Ahoj, Azo");
}

void loop() {
  Serial.println(digitalRead(7));
  delay(50);
}
