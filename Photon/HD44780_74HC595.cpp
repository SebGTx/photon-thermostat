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
  delay(50);

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
    switch (value.charAt(i)) {
      case '/': LCDdata(LCD_SLA); break;
      case '0': LCDdata(LCD_0); break;
      case '1': LCDdata(LCD_1); break;
      case '2': LCDdata(LCD_2); break;
      case '3': LCDdata(LCD_3); break;
      case '4': LCDdata(LCD_4); break;
      case '5': LCDdata(LCD_5); break;
      case '6': LCDdata(LCD_6); break;
      case '7': LCDdata(LCD_7); break;
      case '8': LCDdata(LCD_8); break;
      case '9': LCDdata(LCD_9); break;
      case ':': LCDdata(LCD_DP); break;
      case ';': LCDdata(LCD_PV); break;
      case '<': LCDdata(LCD_INF); break;
      case '=': LCDdata(LCD_EGA); break;
      case '>': LCDdata(LCD_SUP); break;
      case '?': LCDdata(LCD_INT); break;
      case '@': LCDdata(LCD_ARO); break;
      case 'A': LCDdata(LCD_A); break;
      case 'B': LCDdata(LCD_B); break;
      case 'C': LCDdata(LCD_C); break;
      case 'D': LCDdata(LCD_D); break;
      case 'E': LCDdata(LCD_E); break;
      case 'F': LCDdata(LCD_F); break;
      case 'G': LCDdata(LCD_G); break;
      case 'H': LCDdata(LCD_H); break;
      case 'I': LCDdata(LCD_I); break;
      case 'J': LCDdata(LCD_J); break;
      case 'K': LCDdata(LCD_K); break;
      case 'L': LCDdata(LCD_L); break;
      case 'M': LCDdata(LCD_M); break;
      case 'N': LCDdata(LCD_N); break;
      case 'O': LCDdata(LCD_O); break;
      case 'P': LCDdata(LCD_P); break;
      case 'Q': LCDdata(LCD_Q); break;
      case 'R': LCDdata(LCD_R); break;
      case 'S': LCDdata(LCD_S); break;
      case 'T': LCDdata(LCD_T); break;
      case 'U': LCDdata(LCD_U); break;
      case 'V': LCDdata(LCD_V); break;
      case 'W': LCDdata(LCD_W); break;
      case 'X': LCDdata(LCD_X); break;
      case 'Y': LCDdata(LCD_Y); break;
      case 'Z': LCDdata(LCD_Z); break;
      case 'a': LCDdata(LCD_a); break;
      case 'b': LCDdata(LCD_b); break;
      case 'c': LCDdata(LCD_c); break;
      case 'd': LCDdata(LCD_d); break;
      case 'e': LCDdata(LCD_e); break;
      case 'f': LCDdata(LCD_f); break;
      case 'g': LCDdata(LCD_g); break;
      case 'h': LCDdata(LCD_h); break;
      case 'i': LCDdata(LCD_i); break;
      case 'j': LCDdata(LCD_j); break;
      case 'k': LCDdata(LCD_k); break;
      case 'l': LCDdata(LCD_l); break;
      case 'm': LCDdata(LCD_m); break;
      case 'n': LCDdata(LCD_n); break;
      case 'o': LCDdata(LCD_o); break;
      case 'p': LCDdata(LCD_p); break;
      case 'q': LCDdata(LCD_q); break;
      case 'r': LCDdata(LCD_r); break;
      case 's': LCDdata(LCD_s); break;
      case 't': LCDdata(LCD_t); break;
      case 'u': LCDdata(LCD_u); break;
      case 'v': LCDdata(LCD_v); break;
      case 'w': LCDdata(LCD_w); break;
      case 'x': LCDdata(LCD_x); break;
      case 'y': LCDdata(LCD_y); break;
      case 'z': LCDdata(LCD_z); break;
      case ' ': LCDdata(LCD_SPC); break;
      case '°': LCDdata(LCD_DEG); break;
      default: LCDdata(LCD_INT);
    }
  }
}

void HD44780_74HC595::LCDonoff(boolean value){
  if (value == true) {
    LCDcommand(_LCDDisplayMode);
  } else {
    LCDcommand(LCD_DISPLAYOFF);
  }
}

void HD44780_74HC595::SRreset() {
  // Reset 74HC595
  // Enable reset Pin
  digitalWrite(_SRResetPin, LOW);
  delayMicroseconds(10);

  // One clock for reset
  digitalWrite(_SRValidatePin, HIGH);
  delayMicroseconds(10);
  digitalWrite(_SRValidatePin, LOW);
  delayMicroseconds(10);

  // Disable reset Pin
  digitalWrite(_SRResetPin, HIGH);
  delayMicroseconds(10);
}

void HD44780_74HC595::LCDcommand(uint8_t value) {
  // Reset 74HC595
  SRreset();

  // Register Select = Command
  digitalWrite(_LCDRegisterSelectPin, LOW);
  delayMicroseconds(10);

  // Send value
  shiftOut(_SRDataPin, _SRNextValuePin, MSBFIRST, value);
  digitalWrite(_SRValidatePin, HIGH);
  delayMicroseconds(10);
  digitalWrite(_SRValidatePin, LOW);
  digitalWrite(_LCDValidatePin, HIGH);
  delayMicroseconds(40);
  digitalWrite(_LCDValidatePin, LOW);
  delay(10);
}

void HD44780_74HC595::LCDdata(uint8_t value) {
  // Reset 74HC595
  SRreset();

  // Register Select = Data
  digitalWrite(_LCDRegisterSelectPin, HIGH);
  delayMicroseconds(10);

  // Send value
  shiftOut(_SRDataPin, _SRNextValuePin, MSBFIRST, value);
  digitalWrite(_SRValidatePin, HIGH);
  delayMicroseconds(10);
  digitalWrite(_SRValidatePin, LOW);
  digitalWrite(_LCDValidatePin, HIGH);
  delayMicroseconds(40);
  digitalWrite(_LCDValidatePin, LOW);
  delay(5);
}
