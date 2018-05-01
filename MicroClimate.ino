#include <DHT.h>
#include <DHT_U.h>
#include <Nokia_5110.h>  // for the Nolia display library 
#include <DS3231.h>

#define DHTPIN 8
#define LCDBACKLIGHT 9
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
#define RST 3
#define CE 4
#define DC 5
#define DIN 6
#define CLK 7
/*
  Environment monitor
*/

// Globals and display init
Nokia_5110 lcd = Nokia_5110 (RST, CE, DC, DIN, CLK);
DHT dht(DHTPIN, DHTTYPE);
// Init the DS3231 using the hardware interface
DS3231 rtc(SDA, SCL);

// the setup function runs once when you press reset or power the board
void setup() {
  // init RTC
  rtc.begin();
  // init LCD
  pinMode(LCDBACKLIGHT, OUTPUT);
  lcd.clear();
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
    for (int i = 0; i < 9; i++) {
      int b = Serial.read();
      if (i == 2) result = b * 256;
      if (i == 3) result = result + b;
    }
    // myGLCD.printNumI(result, 32, 16, 4);

    // warn by backlight if CO2 is too high
    if (result > 1299) {
      analogWrite(LCDBACKLIGHT, 255);
    }
    if (result <= 1299) {
      analogWrite(LCDBACKLIGHT, 0);
    }
  } else {
    // read bytes to /dev/null
    for (int i = 0; i < buffer; i++) {
      int n = Serial.read();
    }
  }

  getCO2Concentration();
  // print humidity and temperature
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  //myGLCD.printNumF(t, 1, 32, 0);
  //myGLCD.printNumF(h, 1, 32, 8);

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
  // Temperature symbol
  lcd.setCursor(0, 0);
  lcd.print("\004");
  lcd.setCursor(0, 1);
  lcd.print("\006");
  lcd.setCursor(6, 0);
  lcd.print("\005");
  lcd.setCursor(6, 1);
  lcd.print("\007");
  // Degree symbol
  lcd.setCursor(12, 0);
  lcd.print("\001C");
  // RH symbol
  lcd.setCursor(40, 0);
  lcd.print("\010"); // 8 in octal
  lcd.setCursor(40, 1);
  lcd.print("\020"); // 10 in octal
  lcd.setCursor(46, 0);
  lcd.print("\011"); 
  lcd.print("H");
  lcd.setCursor(46, 1);
  lcd.print("\021");
  // CO2 string
  lcd.setCursor(0, 3);
  lcd.print ("CO");
  lcd.setCursor(12, 3);
  lcd.print ("\002"); // subscipt top
  lcd.setCursor(12, 4);
  lcd.print ("\003"); // subscript bottom
}

void displayUpdate(void)
// get time and display it
{
  //  myGLCD.setFont(SmallFont);
  //  myGLCD.print(rtc.getTimeStr(), 0, 35);
  //  myGLCD.setFont(SmallFont);
}


