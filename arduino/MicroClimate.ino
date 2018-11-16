
/*
 * Environment monitor 
 */

#include <DHT.h>
#include <DHT_U.h>
#include <Nokia_5110.h>
#include <DS3231.h>

// PIN assignments
#define DHTPIN 8
#define LCDBACKLIGHT 9
#define DHTTYPE DHT22
#define RST 13
#define CE 4
#define DC 5
#define DIN 6
#define CLK 7
#define CO2WARN 1499
#define CO2ALERT 2001
#define BTN1 2
#define BTN2 3
#define BUZZER 12

/*  Global variables */
Nokia_5110 lcd = Nokia_5110 (RST, CE, DC, DIN, CLK);
DHT dht(DHTPIN, DHTTYPE);
DS3231 rtc(SDA, SCL);

// global loop counter
unsigned int counter = 0;

// button states
int btn1state = LOW;
int btn2state = LOW;
int debounce_count = 15;
int debounce_delay = 50;

// main application states
boolean clock = false;
boolean backlight = false;
boolean co2_warn = false;
boolean co2_alert = false;
unsigned int backlightCounter = 0;
unsigned int soundCounter = 0;

// clock set-up state
unsigned int _position = 0;
char clockValue[] = "00:00:00";

/**
 *  Global set-up function. Runs once
 */
void setup() {
  // init pins
  pinMode(LCDBACKLIGHT, OUTPUT);
  pinMode(BTN1, INPUT);
  pinMode(BTN2, INPUT);
  pinMode(BUZZER, OUTPUT);

  // init CO2 serial
  Serial.begin(9600);
  // init RTC
  rtc.begin();
  // init temperature/humidity sensor
  dht.begin();

  // clear and draw
  lcd.clear();
  loadMainTemplate();
}

/**
 *  This is main loop that runs forever
 */
void loop() {
  // There are 2 modes with 2 different screens: main mode and clock set-up mode
  if (!clock) {
    displayClockUpdate();

    // update measurements every ~7,5 sec
    if (counter % 25 == 0) {
      getCO2Concentration();
      printCO2Concentration();
      printTRH();
    }

    // read buttons
    btn1state = buttonRead(BTN1);
    btn2state = buttonRead(BTN2);

    if (btn1state == HIGH && btn2state == HIGH) {
      clock_mode();
    }

    if (btn1state == HIGH && btn2state == LOW) {
      backlight = true;
      backlightCounter = counter;
    }

    if (btn1state == LOW && btn2state == HIGH) {
      sound = true;
    }
    // if backlight button is pressed turn on backlight for 5 sec.
    if ((backlight && (counter >= backlightCounter && counter - backlightCounter < 15)) || co2_warn) {
      analogWrite(LCDBACKLIGHT, 255);
    } else {
      backlight = false;
      analogWrite(LCDBACKLIGHT, 0);
    }

    // if buzzer button is pressed generate tone for 2 sec.
    if (sound) {
      digitalWrite(BUZZER, HIGH);
      delay(2000);
    }
  } else {
    clock_setup(counter % 3);
  }
  counter++;
  delay(300);
}

/**
 *  RTC clock set-up 
 */
void clock_setup(int blink) {
  // Clock set-up mode
  lcd.setCursor(0, 2);
  // simulate blink at the position
  if (blink == 0) {
    lcd.print(clockValue);
  } else {
    String partial = String(clockValue).substring(0, _position) + String('-') + String(clockValue).substring(_position + 1);
    lcd.print(partial);
  }

  // read buttons
  btn1state = buttonRead(BTN1);
  btn2state = buttonRead(BTN2);

  // exit clock mode
  if ((btn1state == HIGH && btn2state == HIGH) || _position == 8) {
    clock = false;
    if (_position == 8) {
      String _hh = String(clockValue[0]) + String(clockValue[1]);
      String _mm = String(clockValue[3]) + String(clockValue[4]);
      String _ss = String(clockValue[6]) + String(clockValue[7]);
      rtc.setTime(_hh.toInt(), _mm.toInt(), _ss.toInt());
    }
    lcd.clear();
    loadMainTemplate();
  }

  // increase number
  if (btn1state == HIGH && btn2state == LOW) {
    int value = clockValue[_position] - '0';
    value++;
    if (value == 10) value = 0;
    clockValue[_position] = char(value + 48);
  }

  // increase position
  if (btn1state == LOW && btn2state == HIGH) {
    _position++;
    if (_position == 2 || _position == 5) _position++;
  }
}

/**
   Print temperature and humidity from DHT sensor
*/
void printTRH() {
  lcd.setCursor(16, 1);
  char outstr[4];
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  dtostrf(t, 4, 1, outstr);
  lcd.print(outstr);
  dtostrf(h, 4, 1, outstr);
  lcd.setCursor(54, 1);
  lcd.print(outstr);
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

    // warn by backlight if CO2 is moderate
    if (result > CO2WARN) {
      co2_warn = true;
    } else {
      co2_warn = false;
    }

    // alert by sound if CO2 is too high
    if (result > CO2ALERT) {
      co2_warn = true;
    } else {
      co2_warn = false;
    }

  } else {
    // read bytes to /dev/null
    for (int i = 0; i < buffer; i++) {
      int n = Serial.read();
    }
  }
}

/**
 *   Send command to CO2 sensor to get conentration
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
 *  Read RTC and update LCD with clock value
 */
void displayClockUpdate() {
  lcd.setCursor(20, 5);
  lcd.print(rtc.getTimeStr());
}

/**
 *  Button software debounce read procedure
 *  return: button read value
 */
int buttonRead(int button) {
  int count_samples = 0;
  long startTime = millis();
  int lastState = digitalRead(button);
  int reading = lastState;

  while (true) {
    reading = digitalRead(button);

    if (reading != lastState && count_samples > 0) {
      count_samples--;
    }

    if (reading == lastState) {
      count_samples++;
    }

    if (count_samples >= debounce_count) {
      return reading;
    }

    lastState = reading;
  }
}

/**
 *  Clock set-up mode
 */
void clock_mode() {
  clock = true;
  _position = 0;
  lcd.clear();
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
