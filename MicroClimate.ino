#include <DHT.h>
#include <DHT_U.h>
#include <LCD5110_Basic.h>  // for the Nolia display library 
#include <DS3231.h>

#define DHTPIN 8     
#define LCDBACKLIGHT 9
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
/*
  Environment monitor

*/

// Globals
// display
LCD5110 myGLCD(7,6,5,3,4);
DHT dht(DHTPIN, DHTTYPE);
 // Init the DS3231 using the hardware interface
DS3231  rtc(SDA, SCL);

extern uint8_t SmallFont[];
extern uint8_t MediumNumbers[];
extern uint8_t BigNumbers[];

// the setup function runs once when you press reset or power the board
void setup() {
  // init RTC
  rtc.begin();
  // init LCD
  pinMode(LCDBACKLIGHT, OUTPUT);
  myGLCD.InitLCD();
  myGLCD.clrScr();
  loadTemplate();

  // init CO2 serial
  Serial.begin(9600);

  // init temperature/humidity sensor
  dht.begin();

}

// the loop function runs over and over again forever
void loop() {     
  int buffer = Serial.available();
  if (buffer == 9) {
    int result = 0;
    for (int i=0; i<9; i++) {
      int b = Serial.read();
      if (i==2) result = b * 256;
      if (i==3) result = result + b;
    }
    myGLCD.printNumI(result, 32, 16, 4);
    
    // warn by backlight if CO2 is too high
    if (result > 1299) {
      analogWrite(LCDBACKLIGHT, 255);
    }
    if (result <= 1299) {
      analogWrite(LCDBACKLIGHT, 0);
    }  
  } else {
    // read bytes to /dev/null
    for (int i=0; i<buffer; i++) {
      int n = Serial.read();
    }
  }
   
   getCO2Concentration();
   // print humidity and temperature
   float t = dht.readTemperature();
   float h = dht.readHumidity();
   myGLCD.printNumF(t, 1, 32, 0);
   myGLCD.printNumF(h, 1, 32, 8);

   // Check RTC
   displayUpdate();
   
   delay (5000); 
}

void getCO2Concentration() {
   Serial.write(0xFF);
   Serial.write(0x01);
   Serial.write(0x86);
   Serial.write(0x00);
   Serial.write(0x00);
   Serial.write(0x00);
   Serial.write(0x00);
   Serial.write(0x00);
   Serial.write(0x79);
}

void loadTemplate() {
   myGLCD.setFont(SmallFont);
   myGLCD.print("Temp:", 0, 0);
   myGLCD.print("C", 60, 0);
   myGLCD.print("Hum :", 0, 8);
   myGLCD.print("%", 60, 8);
   myGLCD.print("CO   :", 0, 16);
   myGLCD.print("2", 12, 23);
   myGLCD.print("ppm", 60, 16);
}

void displayUpdate(void)
// get time and display it
{
  myGLCD.setFont(SmallFont);
  myGLCD.print(rtc.getTimeStr(), 0, 35); 
  myGLCD.setFont(SmallFont);
}


