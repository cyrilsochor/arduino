#include <IRremote.h>

int POWER_LED_PIN = 2;
int POWER_RELE_PIN = 3;
int RECV_PIN = 11; // vstup pro IR cidlo
unsigned long REMOTE_POWER_CODES[] = {2148500519, 2148467751, // stribrne ovladani, cerveny vlevo nahore
                                      3772790473 // male cerne ovladani, cervene A
                                      }; 
int REMOTE_POWER_CODES_COUNT = sizeof(REMOTE_POWER_CODES)/sizeof(long);

int DELAY_AFTER_SWITCH = 500;

IRrecv irrecv(RECV_PIN);
decode_results results;

boolean power = false;

void setup()
{
  Serial.println("setup()");
  pinMode(POWER_LED_PIN, OUTPUT);
  switchPowerLed();

  pinMode(POWER_RELE_PIN, OUTPUT);
  digitalWrite(POWER_RELE_PIN, HIGH); // nejde proud

  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {

  if (irrecv.decode(&results)) {
    Serial.println(results.value);

    for (int i = 0; i < REMOTE_POWER_CODES_COUNT; i++) {
      if (results.value == REMOTE_POWER_CODES[i]) {
        Serial.println("SWITCH");

        power = ! power;
        switchPowerLed();
        /* tento tanec se delal pro cerneho psa, ktery se ovladal cernym dratovy ovladanim
        if (power) {
          digitalWrite(POWER_RELE_PIN, LOW);// jde proud
          delay(60);
          digitalWrite(POWER_RELE_PIN, HIGH);// nejde proud
          delay(60);
          digitalWrite(POWER_RELE_PIN, LOW);// jde proud
        } else {
          digitalWrite(POWER_RELE_PIN, HIGH);// nejde proud
        }
        */

        if (power) {
          digitalWrite(POWER_RELE_PIN, LOW);// jde proud
        } else {
          digitalWrite(POWER_RELE_PIN, HIGH);// nejde proud
        }

        delay(DELAY_AFTER_SWITCH);
        
        Serial.println("AFTER DELAY");
        break;
      }
    }

    irrecv.resume(); // Receive the next value
  }
  delay(10);
}


void switchPowerLed() {
  if (power) {
    digitalWrite(POWER_LED_PIN, HIGH);
  } else {
    digitalWrite(POWER_LED_PIN, LOW);
  }
}

