boolean DigitOn = LOW;
boolean DigitOff = HIGH;
boolean SegOn=HIGH;
boolean SegOff=LOW;

int PortDigStart = 2;
// digit0 je dvojtecka
int DigitsCount = 5;

int PortSegStart = 20;
int SegsCount = 8;

int digit = 0;

char serialBuff[500];

void setup() {
  for(int i=0; i<DigitsCount; i++){ // od 0 kvuli dvojtecce
    pinMode(PortDigStart + i, OUTPUT);
    digitalWrite(PortDigStart + i, DigitOn);
  }
  for(int i=0; i<=SegsCount; i++){ // = je tam kvuli dvojtecce
    pinMode(PortSegStart + i, OUTPUT);
    digitalWrite(PortSegStart + i, SegOff);
  } 
  Serial.begin(9600);
  pinMode(7, INPUT_PULLUP);
  Serial.println("Ahoj, Azo");
}

void loop() {
  delay(500);
  //sprintf(serialBuff, "Resim digit %i", digit); Serial.println(serialBuff);

  for(int i=0; i<DigitsCount; i++){ // od 0 kvuli dvojtecce
    
    if(i==digit){
     //sprintf(serialBuff, "ON  %i", i); Serial.println(serialBuff);
     digitalWrite(PortDigStart + i, DigitOn);
    } else {
     //sprintf(serialBuff, "OFF %i", i); Serial.println(serialBuff);
     digitalWrite(PortDigStart + i, DigitOff);
    }
  }
  if(digit==0){
    // dvojtecka
  } else {
//    for(int i=0; i<SegsCount; i++){
//      digitalWrite(PortSegStart + i, DigitOn);
//    } 
  }
  
  //digitalWrite(PortSegA, SegOn);
  //digitalWrite(4,SegOn);
//  digitalWrite(2,DigitOn);
//  digitalWrite(3,SegOff);
//  digitalWrite(4,SegOff);
//  delay(200);

  //next digit
  digit++;
  if(digit>DigitsCount) digit=0;
}
