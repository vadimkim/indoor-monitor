/*********************************************************************
Indoor monitor v2.0 based on ESP8266, BME280 and CCS811 sensors 
*********************************************************************/

#include <Nokia_5110.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_CCS811.h>

#define RST D0
#define CE D4
#define DC D5
#define DIN D3
#define CLK D6

#define SEALEVELPRESSURE_HPA (1013.25)

/*  Global variables */
Nokia_5110 lcd = Nokia_5110 (RST, CE, DC, DIN, CLK);
Adafruit_BME280 bme; // BME280
Adafruit_CCS811 ccs; // CCS811

unsigned int counter = 0;
 
void setup()   {
  Serial.begin(9600);
  
  bool status = bme.begin(0x76);

  if (!status) {
    Serial.println("Couldn't find BME280 sensor");
    while (1);  
  }

  if(!ccs.begin()){
    Serial.println("Failed to start CCS811.");
    while(1);
  }

  // Wait for the CCS811 to be ready
  while(!ccs.available());
  
  lcd.clear();
  loadMainTemplate();
}
 
 
void loop() {
  
    // update measurements every ~7,5 sec
    if (counter % 25 == 0) {
      getCO2Concentration();
      printCO2Concentration();
      printBme280();
      printCCS811();
    }

  counter++;
  delay(300);
}

/**
 * Print CCS811 data 
 */
void printCCS811() {
  lcd.setCursor(0, 5);
  if (!ccs.readData()) {
    lcd.print("CO2: ");
    lcd.print(ccs.geteCO2());
    lcd.print(" : ");
    lcd.print(ccs.getTVOC());
    lcd.print(" pp  "); 
  }
}

/**
 * Print BME280 data 
 */
void printBme280() {
  lcd.setCursor(16, 1);
  char outstr[4];
  float t = bme.readTemperature();
  float h = bme.readHumidity();
  dtostrf(t, 4, 1, outstr);
  lcd.print(outstr);
  dtostrf(h, 4, 1, outstr);
  lcd.setCursor(54, 1);
  lcd.print(outstr);
}
 
/**
 * Send command to CO2 sensor to get conentration
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

/**
 *   Read CO2 concentration from serial port and print to LCD
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
    lcd.print(" ppm   ");

//    // warn by backlight if CO2 is moderate
//    if (result > CO2WARN) {
//      co2_warn = true;
//    } else {
//      co2_warn = false;
//    }

//    // alert by sound if CO2 is too high
//    if (result > CO2ALERT) {
//      co2_alert = true;
//    } else {
//      co2_alert = false;
//    }

  } else {
    // read bytes to /dev/null
    for (int i = 0; i < buffer; i++) {
      int n = Serial.read();
    }
  }
}

/**
 *  Screen templates
 */
void loadMainTemplate() {
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
