// see https://arduino.cz/tutorial-uzivani-hodin-realneho-casu-ds1307-a-ds3231-s-arduinem/

#include <SPI.h> // required for small oled
#include "Wire.h" // required for clock module
#include <OneWire.h> // temp sensor DS18B20
#include <DallasTemperature.h> // temp sensor DS18B20
#include <Adafruit_GFX.h> // required for small oled
#include <Adafruit_SSD1306.h> // required for small oled

#define DS3231_I2C_ADDRESS 0x68 // required for clock module

OneWire oneWireDS(7); // 7 cislo pinu teplotniho cidla
// vytvoření instance senzoryDS z knihovny DallasTemperature
DallasTemperature senzoryDS(&oneWireDS);

byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;

// small oled variables
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 
#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void setup(){
    setupClock();
    setupTemperature();
    setupSmallOled();
    
    Serial.begin(9600);
    
    // set the initial time here:
    // DS3231     ss,mm,HH,dw,dd,MM,YY
    //setDS3231time(00,42,06,04,04,05,17);
}

void setupClock(){
  Wire.begin();
}

void setupTemperature(){
  senzoryDS.begin();
}

void setupSmallOled(){
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.clearDisplay();
  display.setTextColor(WHITE);
}

void loop(){
  int val = analogRead(A0);
  float mv = ( val/1024.0)*5000; 
  float cel = mv/10;
  float farh = (cel*9)/5 + 32;
  
  Serial.print("TEMPRATURE = ");
  Serial.print(cel);
  Serial.print("*C");
  Serial.println();

  //displayTimeSerial(); // display the real-time clock data on the Serial Monitor,
  //displayTempSerial();
  
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  senzoryDS.requestTemperatures();

  display.clearDisplay();
  displayTimeSmallOled();
  displayTempSmallOled();
  display.display();
 
  //delay(1000);
}

char timestring[3];
void displayTimeSmallOled(){

  //display hour
  display.setTextSize(5);
  display.setCursor(0,0);
  sprintf(timestring,"%2u", hour);
  display.print(timestring);

  // display calibration pixel
  //display.drawPixel(127, 40, WHITE);
  display.fillCircle(60,10,2, WHITE);
  display.fillCircle(60,25,2, WHITE);

  // display minute
  display.setCursor(67,0);
  display.setTextSize(5);
  sprintf(timestring,"%.2u", minute);
  display.print(timestring);
}

char tempstring[7];
void displayTempSmallOled(){
  // display current temp
  display.setTextSize(2);
  display.setCursor(00,47);
  float temp = senzoryDS.getTempCByIndex(0);
  tempstring[0]=(temp>=0)?'+':'-';
  dtostrf(temp, 2, 1, tempstring+1);
  //sprintf(tempstring,"%+2.1f", -12.3);
  display.print(tempstring);  
}

void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year){
    // sets time and date data to DS3231
    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(0); // set next input to start at the seconds register
    Wire.write(decToBcd(second)); // set seconds
    Wire.write(decToBcd(minute)); // set minutes
    Wire.write(decToBcd(hour)); // set hours
    Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
    Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
    Wire.write(decToBcd(month)); // set month
    Wire.write(decToBcd(year)); // set year (0 to 99)
    Wire.endTransmission();
}

void readDS3231time(byte *second, byte *minute, byte *hour, byte *dayOfWeek, byte *dayOfMonth, byte *month, byte *year){
    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(0); // set DS3231 register pointer to 00h
    Wire.endTransmission();
    Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
    // request seven bytes of data from DS3231 starting from register 00h
    *second = bcdToDec(Wire.read() & 0x7f);
    *minute = bcdToDec(Wire.read());
    *hour = bcdToDec(Wire.read() & 0x3f);
    *dayOfWeek = bcdToDec(Wire.read());
    *dayOfMonth = bcdToDec(Wire.read());
    *month = bcdToDec(Wire.read());
    *year = bcdToDec(Wire.read());
}

void displayTimeSerial(){
    byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
    // retrieve data from DS3231
    readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
    // send it to the serial monitor
    Serial.print(hour, DEC);
    // convert the byte variable to a decimal number when displayed
    Serial.print(":");
    if (minute<10){
        Serial.print("0");
    }
    Serial.print(minute, DEC);
    Serial.print(":");
    if (second<10){
        Serial.print("0");
    }
    Serial.print(second, DEC);
    Serial.print(" ");
    Serial.print(dayOfMonth, DEC);
    Serial.print("/");
    Serial.print(month, DEC);
    Serial.print("/");
    Serial.print(year, DEC);
    Serial.print(" Day of week: ");
    switch(dayOfWeek){
        case 1:
            Serial.println("Sunday");
            break;
        case 2:
            Serial.println("Monday");
            break;
        case 3:
            Serial.println("Tuesday");
            break;
        case 4:
            Serial.println("Wednesday");
            break;
        case 5:
            Serial.println("Thursday");
            break;
        case 6:
            Serial.println("Friday");
            break;
        case 7:
            Serial.println("Saturday");
            break;
    }
}

void displayTempSerial(){
  Serial.print("Teplota cidla DS18B20: ");
  Serial.print(senzoryDS.getTempCByIndex(0));
  Serial.println(" stupnu Celsia");
}

// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val){
    return( (val/10*16) + (val%10) );
}

// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val){
    return( (val/16*10) + (val%16) );
}

