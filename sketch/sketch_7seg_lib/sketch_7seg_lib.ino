#include <SevSeg.h>


SevSeg sevseg;

void setup() {
  byte numDigits = 4;
  byte digitPins[] = {3, 4, 5, 6};
  byte segmentPins[] = {30, 31, 32, 33, 34, 35, 36, 37};
  sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins, true, false);
  sevseg.setNumber(9012);
}

void loop() {
  
   sevseg.refreshDisplay();
}
