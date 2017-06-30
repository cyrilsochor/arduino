#include <IRremote.h>

int RECV_PIN = 11; // vstup pro IR cidlo
int POWER_LED_PIN = 2;
unsigned long REMOTE_POWER_VALUE = 1906441864;
int DELAY_AFTER_SWITCH = 1000;

IRrecv irrecv(RECV_PIN);
decode_results results;

boolean power = true;

void setup()
{
  pinMode(POWER_LED_PIN, OUTPUT);
  switchPowerLed();
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  
  if (irrecv.decode(&results)) {
    Serial.println(results.value);
    if(results.value == REMOTE_POWER_VALUE){
      Serial.println("SWITCH");
      power = ! power;
      switchPowerLed();      
      delay(DELAY_AFTER_SWITCH);      
      Serial.println("AFTER DELAY");
    }
    
    irrecv.resume(); // Receive the next value
  }
  delay(10);
}


void switchPowerLed(){
  if(power){
    digitalWrite(POWER_LED_PIN, HIGH);
  } else {
    digitalWrite(POWER_LED_PIN, LOW);
  }
}

