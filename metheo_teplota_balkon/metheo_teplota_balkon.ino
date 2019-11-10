#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile
#include <OneWire.h> // temp sensor DS18B20
#include <DallasTemperature.h> // temp sensor DS18B20

// urceno pro Arduino NANO (Board: Arduino Nano, Processor: ATmega168)
// zapojeni PIN 12 - do vysilace (ten mensi) druhy pin, prvni pin volny, treti 5V, ctvrty GND
// zapojine PIN 7  - vstup z teplotniho cidla (externi, cerny kabel)

RH_ASK driver; // RadioHead knihovna RadioHead MHz

OneWire oneWireDS(7); // 7 cislo pinu teplotniho cidla
// vytvoření instance senzoryDS z knihovny DallasTemperature
DallasTemperature senzoryDS(&oneWireDS);

float temp0;

void setup(){
    Serial.begin(9600);   // Debugging only
    if (!driver.init())
         Serial.println("radio init failed");
    else 
         Serial.println("radio init success");
    temp0 = -99;
    setupTemperature();

    // dalsi napajeni (Nano ma jen jeden 5V a nechci mit desku kvuli uspore mista)
    pinMode(2, OUTPUT);
    digitalWrite(2, HIGH);
    pinMode(3, OUTPUT);
    digitalWrite(3, HIGH);
}


void loop(){
  //displayTempSerial();
  float temp0Act = senzoryDS.getTempCByIndex(0);
  if( temp0Act > -100 ) {// obcas to dava teplotu -127, kterou chci odfiltrovat 
    temp0 = temp0Act;
  }

  char float_buff[7];  
  dtostrf(temp0, 4, 2, float_buff);// sprintf na arduinu neumi %f, musime pouzit takto
  char msg[64];
  snprintf(msg, sizeof(msg), "SOCH_TEMP_0: %s stupnu", float_buff);
  driver.send((uint8_t *)msg, strlen(msg));
  driver.waitPacketSent();
  Serial.println(msg);
  delay(1000);
}

void setupTemperature(){
  senzoryDS.begin();
}

void displayTempSerial(){
  Serial.print("Teplota cidla DS18B20: ");
  Serial.print(senzoryDS.getTempCByIndex(0));
  Serial.println(" stupnu Celsia");
}

