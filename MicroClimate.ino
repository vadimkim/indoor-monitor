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
DS3231 rtc(SDA, SCL);
// global loop counter
unsigned int counter = 0;
bool backlight = false;

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
  // update measurements every 5 sec
  if (counter % 10 == 0) {
    getCO2Concentration();
    printCO2Concentration();
    printTRH();
  }
  
  checkButtons();
  displayClockUpdate();
  delay (500);
  counter++;
}

/*
 * Print temperature and humidity
 */
void printTRH() {
  lcd.setCursor(16, 1);
  char outstr[4];
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  dtostrf(t,4,1,outstr);
  lcd.print(outstr);
  dtostrf(h,4,1,outstr);
  lcd.setCursor(54, 1);
  lcd.print(outstr);
}

/*
   Read CO2 concentration from serial port and print to LCD
*/
void printCO2Concentration() {
  int buffer = Serial.available();
  if (buffer == 9) {
    int result = 0;
    for (int i = 0; i < 9; i++) {
      int b = Serial.read();
      if (i == 2) result = b * 256;
      if (i == 3) result = result + b;
    }

    lcd.setCursor(24, 3);
    lcd.print(result);
    lcd.print(" ppm ");

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
}

/*
   Send command to CO2 sensor to get conentration
*/
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

/**
 * Read RTC and update LCD with clock value
 */
void displayClockUpdate() {
  lcd.setCursor(20, 5);
  lcd.print(rtc.getTimeStr());
}

void checkButtons() {
  // TODO
}

