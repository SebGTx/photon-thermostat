/*
 Photon library to use 4x20 or 2x16 screen with HD44780 controller
 thru a 74HC595 shift register (only 6 pins in 8bits operation)

 HD44780 datasheet : https://www.sparkfun.com/datasheets/LCD/HD44780.pdf
 74HC595 datasheet : https://www.sparkfun.com/datasheets/IC/SN74HC595.pdf

 License: CC BY-SA 3.0: Creative Commons Share-alike 3.0. Feel free
 to use and abuse this code however you'd like. If you find it useful
 please attribute, and SHARE-ALIKE!

 Created July 2015
 by Sébastien Gellet

*/

#ifndef _HD44780_74HC595_H
#define _HD44780_74HC595_H

#include "application.h"

// Debug
#define HD44780_74HC595_SerialDebug false

// Commands
#define LCD_SIZE_2x16     0x30
#define LCD_SIZE_4x20     0x38
#define LCD_CLEARDISPLAY	0x01
#define LCD_RETURNHOME	  0x02
#define LCD_ENTRYMODELEFT 0x06
#define LCD_DISPLAYON     0x0C
#define LCD_DISPLAYONCUR  0x0F
#define LCD_DISPLAYOFF    0x08
#define LCD_MOVECURSOR    0x80

// Data
#define LCD_SPC  0xA0
#define LCD_DEG  0xDF

class HD44780_74HC595 {
  public:
    HD44780_74HC595();

    void
      begin(),
      begin(uint8_t LCDSize),
      begin(int SRDataPin, int SRNextValuePin, int SRValidatePin, int SRResetPin, int LCDRegisterSelectPin, int LCDValidatePin, uint8_t LCDSize),
      print(String value),
      SRreset(),
      LCDonoff(boolean value),
      LCDcommand(uint8_t value),
      LCDdata(uint8_t value);
    bool LCDOn;

  private:
    uint8_t _LCDSize;
    uint8_t _LCDDisplayMode;
    int _SRDataPin;
    int _SRNextValuePin;
    int _SRValidatePin;
    int _SRResetPin;
    int _LCDRegisterSelectPin;
    int _LCDValidatePin;
    bool _LCDWait;
};


#endif
