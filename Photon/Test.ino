#include "HD44780_74HC595.h"
#include "DHT.h"

#define DATE_POS 0
#define TIME_POS 15
#define TEMP_POS 84
#define HUMI_POS 96
#define ONE_DAY_MILLIS (24 * 60 * 60 * 1000)
#define ONE_HOUR_MILLIS (60 * 60 * 1000)
#define ONE_MIN_MILLIS (60 * 1000)
#define SCREEN_TIME_AWAKE 60
unsigned long lastSync = millis();
unsigned long lastSensor = millis() - ONE_DAY_MILLIS;
int screen_disable = D6;
int wakeupbutton = D7;

DHT dht = DHT(A0, DHT22);
HD44780_74HC595 lcd = HD44780_74HC595();

STARTUP(startup());

// Value stored in backup ram
retained String lastTime;
retained String lastDay;
retained String lastTemperature;
retained String lastHumidity;
retained String lastTemp;

void startup() {
  System.enableFeature(FEATURE_RETAINED_MEMORY);

  Time.zone(+1);

  //lcd.begin(LCD_SIZE_4x20);
}

// Volatile value
boolean screenState = true;
boolean wakeUpButton = false;
time_t sleepScreenTime;

void setup() {
  Serial.begin(9600);
  Serial.println("Photon Serial Port is working!");

  dht.begin();
  lcd.begin(LCD_SIZE_4x20);

  pinMode(screen_disable, OUTPUT);
  digitalWrite(screen_disable, HIGH);
  pinMode(wakeupbutton, INPUT);

  Particle.function("movecur", moveFromWeb);
  Particle.function("print", printFromWeb);
  Particle.function("clear", clearFromWeb);

  sleepScreenTime = Time.now() + SCREEN_TIME_AWAKE;

  //bool success;
  //success = Particle.publish("lastTemperature", String(lastTemperature));
  /*success = Particle.publish("lastTemperature", lastDay);
  success = Particle.publish("lastTemperature", lastTime);
  success = Particle.publish("lastTemperature", lastTemp);*/
  /*Serial.println(lastDay);
  Serial.println(lastTime);
  Serial.println(lastTemperature);
  Serial.println(lastTemp);*/

  attachInterrupt(wakeupbutton, changeWakeUpButton, CHANGE);
}

void changeWakeUpButton() {
  boolean buttonState = digitalRead(wakeupbutton);
  if (wakeUpButton == buttonState) { return; }
  wakeUpButton = buttonState;
  if (buttonState == true) {
    sleepScreenTime = Time.now() + SCREEN_TIME_AWAKE;
  }
}

void tempoAffichage() {
  time_t maintenant = Time.now();
  if (maintenant >= sleepScreenTime) {
    lcd.LCDonoff(false);
    digitalWrite(screen_disable, LOW);
  } else {
    lcd.LCDonoff(true);
    digitalWrite(screen_disable, HIGH);
  }
}

void printDate(int position) {
  // Day change
  uint8_t tmppos = position;
  time_t maintenant = Time.now();
  String curDay = Time.format(maintenant, "%d/%m/%Y");
  if (curDay != lastDay) {
    lcd.LCDcommand(LCD_MOVECURSOR | tmppos);
    lcd.print(curDay);
    lastDay = curDay;
  }
}

void printTime(int position) {
  // Time change
  uint8_t tmppos = position;
  time_t maintenant = Time.now();
  String curTime = Time.format(maintenant, "%k:%M");
  if (curTime != lastTime) {
    lcd.LCDcommand(LCD_MOVECURSOR | tmppos);
    lcd.print(curTime);
    lastTime = curTime;
  }
}

bool printTemp(int position) {
  uint8_t tmppos = position;
  float t = dht.readTemperature();
  Serial.println(t);

  if (t==NAN) {
    return false;
  } else {
    String strTemp(t, 1);
    if (strTemp != lastTemperature) {
      lastTemperature = strTemp;
      uint8_t command = LCD_MOVECURSOR | tmppos;
      lcd.LCDcommand(command);
      String temperature = "TEMP:" + strTemp;
      lcd.print(temperature);
      lcd.LCDdata(LCD_DEG);
      // Publish new Value
      bool success;
      success = Particle.publish("lastTemperature", strTemp);
    }
    return true;
  }
}

bool printHumidity(int position) {
  uint8_t tmppos = position;
  float h = dht.readHumidity();
  Serial.println(h);

  if (h==NAN) {
    return false;
  } else {
    String strHumidity(h, 0);
    if (strHumidity != lastHumidity) {
      lastHumidity = strHumidity;
      uint8_t command = LCD_MOVECURSOR | tmppos;
      lcd.LCDcommand(command);
      String humidity = "HUMI:" + strHumidity;
      lcd.print(humidity);
      lcd.LCDdata(LCD_PER);
      // Publish new Value
      bool success;
      success = Particle.publish("lastHumidity", strHumidity);
    }
    return true;
  }
}

void clearScreen() {
  uint8_t command = LCD_CLEARDISPLAY;
  lcd.LCDcommand(command);
  printDate(DATE_POS);
  printTime(TIME_POS);
}

void loop() {
  // Time sync
  if (millis() - lastSync > ONE_DAY_MILLIS) {
    // Request time synchronization from the Particle Cloud
    Particle.syncTime();
    lastSync = millis();
  }
  // Get Now Date And Time
  printDate(DATE_POS);
  printTime(TIME_POS);
  // Temp and humidity every one minute
  if (millis() - lastSensor > ONE_MIN_MILLIS) {
    // Get Temperature and humidity
    bool tempSuccess = printTemp(TEMP_POS);
    bool humiSuccess = printHumidity(HUMI_POS);
    if (tempSuccess && humiSuccess) { lastSensor = millis(); } else {
      bool success;
      success = Particle.publish("Get Humidity or Temperature", "Failed");
    }
  }
  // Tempo Extinction Affichage
  tempoAffichage();

  /*Serial.println(lastDay);
  Serial.println(lastTime);
  Serial.println(lastTemperature);
  Serial.println(lastHumidity);*/

  delay(1000);
}

int printFromWeb(String value) {
  bool success;
  success = Particle.publish("printFromWeb", value);
  lcd.print(value);
  return true;
}

int moveFromWeb(String value) {
  bool success;
  success = Particle.publish("moveFromWeb", value);
  uint8_t command = LCD_MOVECURSOR | value.toInt();
  lcd.LCDcommand(command);
  return true;
}

int clearFromWeb(String value) {
  bool success;
  success = Particle.publish("clearFromWeb", value);
  clearScreen();
  return true;
}
