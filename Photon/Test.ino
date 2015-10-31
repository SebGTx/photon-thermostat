#include "HD44780_74HC595.h"

#define DATE_POS 0
#define TIME_POS 15
#define TEMP_POS 84
#define ONE_DAY_MILLIS (24 * 60 * 60 * 1000)
#define SCREEN_TIME_AWAKE 30
unsigned long lastSync = millis();

HD44780_74HC595 lcd = HD44780_74HC595();

STARTUP(startup());

// Value stored in backup ram
retained String lastTime;
retained String lastDay;
retained float lastTemperature;
retained String lastTemp;

void startup() {
  System.enableFeature(FEATURE_RETAINED_MEMORY);

  Time.zone(+2);

  //lcd.begin(LCD_SIZE_4x20);
}

boolean screenState = true;
boolean wakeUpButton = false;
time_t sleepScreenTime;

void setup() {
  Serial.begin(9600);
  Serial.println("Photonic_Blaster Serial Port is working!");

  lcd.begin(LCD_SIZE_4x20);

  pinMode(D7, OUTPUT);
  digitalWrite(D7, HIGH);
  pinMode(D6, INPUT);

  Particle.function("movecur", moveFromWeb);
  Particle.function("print", printFromWeb);
  Particle.function("clear", clearFromWeb);
  Particle.function("temp", writeTemp);

  sleepScreenTime = Time.now() + SCREEN_TIME_AWAKE;

  bool success;
  success = Particle.publish("lastTemperature", String(lastTemperature));
  /*success = Particle.publish("lastTemperature", lastDay);
  success = Particle.publish("lastTemperature", lastTime);
  success = Particle.publish("lastTemperature", lastTemp);*/
  /*Serial.println(lastDay);
  Serial.println(lastTime);
  Serial.println(lastTemperature);
  Serial.println(lastTemp);*/

  attachInterrupt(D6, changeWakeUpButton, CHANGE);
}

void changeWakeUpButton() {
  boolean buttonState = digitalRead(D6);
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
    digitalWrite(D7, LOW);
  } else {
    lcd.LCDonoff(true);
    digitalWrite(D7, HIGH);
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
  tempoAffichage();
  //int tempValue = lastTemperature * 10;
  //writeTemp(String(tempValue/10));
  /*Serial.println(lastDay);
  Serial.println(lastTime);
  Serial.println(lastTemperature);
  Serial.println(lastTemp);*/

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

int writeTemp(String value) {
  float newTemperature = value.toFloat();
  if (lastTemperature == newTemperature) { return true; }
  lastTemperature = newTemperature;
  bool success;
  success = Particle.publish("lastTemperature", String(lastTemperature));
  uint8_t command = LCD_MOVECURSOR | TEMP_POS;
  lcd.LCDcommand(command);
  String temperature = "TEMP:" + value;
  lcd.print(temperature);
  lcd.LCDdata(LCD_DEG);
  return true;
}
