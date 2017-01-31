#include <Keypad.h>

const byte numRows= 4; //number of rows on the keypad
const byte numCols= 4; //number of columns on the keypad
char keymap[numRows][numCols]= 
{
{'1', '2', '3', 'A'}, 
{'4', '5', '6', 'B'}, 
{'7', '8', '9', 'C'},
{'*', '0', '#', 'D'}
};
byte rowPins[numRows] = {8,7,6,5}; //Rows 0 to 3
byte colPins[numCols]= {4,3,2,A5}; //Columns 0 to 3
Keypad myKeypad= Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);


volatile boolean state = true;
int ledFirst = 9;
int ledLast = 13;
int ledCount = ledLast-ledFirst+1;
int cmax = 1 << ledCount;
int c = 0;

void setup() {
  pinMode(2, INPUT_PULLUP);
  //attachInterrupt(0, pin_ISR, LOW);
  for(int i=ledFirst; i<=ledLast; i++){
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
  Serial.begin(9600);
  myKeypad.addEventListener(keypadEvent); //add an event listener for this keypad
  Serial.print("START:");
}

void loop() {
  char key = myKeypad.getKey();
  if (key) {
    Serial.print("L");
    Serial.println(key);
  }
  
  if(state){
    for(int i=ledFirst; i<=ledLast; i++){
      digitalWrite(i, LOW);
    }
  } else {
    c++;
    if(c==cmax){
      c=0;
    }
    //Serial.print(c);
    //Serial.print(" ? ");
    //Serial.println(cmax);
    
    for(int i=ledFirst; i<=ledLast; i++){
      int ledStatus = c & (1<<(i-ledFirst));
      //Serial.println(ledStatus);
      if( ledStatus > 0){
        digitalWrite(i, HIGH);
      } else {
        digitalWrite(i, LOW);
      }
    }
  }

  delay(300);
}

void pin_ISR(){
    state = !state;
}

void keypadEvent(KeypadEvent key){
  Serial.print("E");
  Serial.println(key);
  switch (myKeypad.getState()){
    case PRESSED:
      switch (key){
        case '*': 
          state = !state;
        break;
      }
    break;
    case RELEASED:
      switch (key){
        case '*': 
        break;
      }
    break;
    case HOLD:
      switch (key){
      }
    break;
  }
}

