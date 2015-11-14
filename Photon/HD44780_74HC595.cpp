#include "HD44780_74HC595.h"

HD44780_74HC595::HD44780_74HC595() {
  _SRDataPin = D0;
  _SRNextValuePin = D1;
  _SRValidatePin = D2;
  _SRResetPin = D3;
  _LCDRegisterSelectPin = D4;
  _LCDValidatePin = D5;
  _LCDSize = LCD_SIZE_2x16;
  _LCDDisplayMode = LCD_DISPLAYON;
  _LCDWait = false;

  LCDOn = true;
}

void HD44780_74HC595::begin() {
  pinMode(_SRDataPin, OUTPUT);
  pinMode(_SRNextValuePin, OUTPUT);
  pinMode(_SRValidatePin, OUTPUT);
  pinMode(_SRResetPin, OUTPUT);
  pinMode(_LCDRegisterSelectPin, OUTPUT);
  pinMode(_LCDValidatePin, OUTPUT);

  // Init all outputs
  digitalWrite(_SRDataPin, LOW);
  digitalWrite(_SRNextValuePin, LOW);
  digitalWrite(_SRValidatePin, LOW);
  digitalWrite(_SRResetPin, LOW);
  digitalWrite(_LCDRegisterSelectPin, LOW);
  digitalWrite(_LCDValidatePin, LOW);
  delayMicroseconds(10);

  // Init 74HC595
  SRreset();

  // Wait 50ms
  delay(15);

  // Init LCD
  LCDcommand(_LCDSize);
  LCDcommand(LCD_CLEARDISPLAY);
  LCDcommand(LCD_ENTRYMODELEFT);
  LCDcommand(_LCDDisplayMode);
}

void HD44780_74HC595::begin(uint8_t LCDSize) {
  _LCDSize = LCDSize;

  begin();
}

void HD44780_74HC595::begin(int SRDataPin, int SRNextValuePin, int SRValidatePin,
                            int SRResetPin, int LCDRegisterSelectPin, int LCDValidatePin,
                            uint8_t LCDSize) {
  _SRDataPin = SRDataPin;
  _SRNextValuePin = SRNextValuePin;
  _SRValidatePin = SRValidatePin;
  _SRResetPin = SRResetPin;
  _LCDRegisterSelectPin = LCDRegisterSelectPin;
  _LCDValidatePin = LCDValidatePin;
  _LCDSize = LCDSize;

  begin();
}

void HD44780_74HC595::print(String value) {
  for (int i = 0; i < value.length(); i++){
    uint8_t charCode = value.charAt(i);
    uint8_t shiftCode7 = charCode >> 7;
    uint8_t shiftCode5 = charCode >> 5;
    if (shiftCode7 == 0x00) {
      // 1 octet UTF-8 Char (https://fr.wikipedia.org/wiki/UTF-8)
      // ASCII table from "Exclamation mark" to "Closing brace"
      if ((charCode >= 0x20) && (charCode <= 0x7D)) {
        LCDdata(charCode);
      } else {
        if (HD44780_74HC595_SerialDebug) { Serial.println("[HD44780_74HC595.cpp] print(" + String(charCode, HEX) + ") non géré"); }
      }
    } else if (shiftCode5 == 0x06) {
      // 2 octets UTF-8 Char
      uint16_t UTF8Hex = charCode << 8;
      i++;
      UTF8Hex = UTF8Hex | ((uint16_t)value.charAt(i));
      switch (UTF8Hex) {
        case 0xC2B0: LCDdata(LCD_DEG); break;
        default:
          if (HD44780_74HC595_SerialDebug) { Serial.println("[HD44780_74HC595.cpp] print(" + String(UTF8Hex, HEX) + ") non géré"); }
      }
    }
  }
}

void HD44780_74HC595::LCDonoff(bool value){
  if (value == true) {
    LCDcommand(_LCDDisplayMode);
    LCDOn = true;
  } else {
    LCDcommand(LCD_DISPLAYOFF);
    LCDOn = false;
  }
}

void HD44780_74HC595::SRreset() {
  // Reset 74HC595
  // Enable reset Pin
  digitalWrite(_SRResetPin, LOW);
  delayMicroseconds(1);

  // One clock for reset
  digitalWrite(_SRValidatePin, HIGH);
  delayMicroseconds(1);
  digitalWrite(_SRValidatePin, LOW);
  //delayMicroseconds(1);

  // Disable reset Pin
  digitalWrite(_SRResetPin, HIGH);
  delayMicroseconds(1);
}

void HD44780_74HC595::LCDcommand(uint8_t value) {
  while (_LCDWait == true) { delayMicroseconds(100); }
  _LCDWait = true;
  if (HD44780_74HC595_SerialDebug) { Serial.println("[HD44780_74HC595.cpp] LCDcommand(" + String(value, HEX) + ") Begin"); }
  // Reset 74HC595
  SRreset();

  // Register Select = Command
  digitalWrite(_LCDRegisterSelectPin, LOW);
  delayMicroseconds(1);

  // Send value
  shiftOut(_SRDataPin, _SRNextValuePin, MSBFIRST, value);
  digitalWrite(_SRValidatePin, HIGH);
  delayMicroseconds(10);
  digitalWrite(_SRValidatePin, LOW);
  digitalWrite(_LCDValidatePin, HIGH);
  delayMicroseconds(40);
  digitalWrite(_LCDValidatePin, LOW);
  delayMicroseconds(700);
  if (HD44780_74HC595_SerialDebug) { Serial.println("[HD44780_74HC595.cpp] LCDcommand(" + String(value, HEX) + ") End"); }
  _LCDWait = false;
}

void HD44780_74HC595::LCDdata(uint8_t value) {
  while (_LCDWait == true) { delayMicroseconds(100); }
  _LCDWait = true;
  if (HD44780_74HC595_SerialDebug) { Serial.println("[HD44780_74HC595.cpp] LCDdata(" + String(value, HEX) + ") Begin"); }
  // Reset 74HC595
  SRreset();

  // Register Select = Data
  digitalWrite(_LCDRegisterSelectPin, HIGH);
  delayMicroseconds(1);

  // Send value
  shiftOut(_SRDataPin, _SRNextValuePin, MSBFIRST, value);
  digitalWrite(_SRValidatePin, HIGH);
  delayMicroseconds(10);
  digitalWrite(_SRValidatePin, LOW);
  digitalWrite(_LCDValidatePin, HIGH);
  delayMicroseconds(40);
  digitalWrite(_LCDValidatePin, LOW);
  delayMicroseconds(50);
  if (HD44780_74HC595_SerialDebug) { Serial.println("[HD44780_74HC595.cpp] LCDdata(" + String(value, HEX) + ") End"); }
  _LCDWait = false;
}
