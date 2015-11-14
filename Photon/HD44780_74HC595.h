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
#define LCD_PER  0x25
#define LCD_VIR  0x2C
#define LCD_PNT  0x2E
#define LCD_SLA  0x2F
#define LCD_0    0x30
#define LCD_1    0x31
#define LCD_2    0x32
#define LCD_3    0x33
#define LCD_4    0x34
#define LCD_5    0x35
#define LCD_6    0x36
#define LCD_7    0x37
#define LCD_8    0x38
#define LCD_9    0x39
#define LCD_DP   0x3A
#define LCD_PV   0x3B
#define LCD_INF  0x3C
#define LCD_EGA  0x3D
#define LCD_SUP  0x3E
#define LCD_INT  0x3F
#define LCD_ARO  0x40
#define LCD_A    0x41
#define LCD_B    0x42
#define LCD_C    0x43
#define LCD_D    0x44
#define LCD_E    0x45
#define LCD_F    0x46
#define LCD_G    0x47
#define LCD_H    0x48
#define LCD_I    0x49
#define LCD_J    0x4A
#define LCD_K    0x4B
#define LCD_L    0x4C
#define LCD_M    0x4D
#define LCD_N    0x4E
#define LCD_O    0x4F
#define LCD_P    0x50
#define LCD_Q    0x51
#define LCD_R    0x52
#define LCD_S    0x53
#define LCD_T    0x54
#define LCD_U    0x55
#define LCD_V    0x56
#define LCD_W    0x57
#define LCD_X    0x58
#define LCD_Y    0x59
#define LCD_Z    0x5A
#define LCD_a    0x61
#define LCD_b    0x62
#define LCD_c    0x63
#define LCD_d    0x64
#define LCD_e    0x65
#define LCD_f    0x66
#define LCD_g    0x67
#define LCD_h    0x68
#define LCD_i    0x69
#define LCD_j    0x6A
#define LCD_k    0x6B
#define LCD_l    0x6C
#define LCD_m    0x6D
#define LCD_n    0x6E
#define LCD_o    0x6F
#define LCD_p    0x70
#define LCD_q    0x71
#define LCD_r    0x72
#define LCD_s    0x73
#define LCD_t    0x74
#define LCD_u    0x75
#define LCD_v    0x76
#define LCD_w    0x77
#define LCD_x    0x78
#define LCD_y    0x79
#define LCD_z    0x7A
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
  //bool;

 private:
   uint8_t _LCDSize;
   uint8_t _LCDDisplayMode;
   int _SRDataPin;
   int _SRNextValuePin;
   int _SRValidatePin;
   int _SRResetPin;
   int _LCDRegisterSelectPin;
   int _LCDValidatePin;
};


#endif
