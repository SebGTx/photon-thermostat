/*
 Photon library to use DHT11/DHT22 temperature/humidity sensor with a photon

 DHT22 datasheet : https://www.sparkfun.com/datasheets/Sensors/Temperature/DHT22.pdf

 License: CC BY-SA 3.0: Creative Commons Share-alike 3.0. Feel free
 to use and abuse this code however you'd like. If you find it useful
 please attribute, and SHARE-ALIKE!

 Created Novembre 2015
 by wgbartley and jdubrow42
 rewrite by Sébastien Gellet

 Original source : https://gist.github.com/wgbartley/8301123
*/

#ifndef _DHT_H
#define _DHT_H

#include "application.h"

// Debug
#define DHT_SerialDebug false


#define MAXTIMINGS 85

#define cli noInterrupts
#define sei interrupts

#define DHT11 11
#define DHT22 22
#define DHT21 21
#define AM2301 21

#define NAN 999999

class DHT {
    private:
        uint8_t data[6];
        uint8_t _pin, _type, _count;
        bool read(void);
        unsigned long _lastreadtime;
        bool firstreading;

    public:
        DHT(uint8_t pin, uint8_t type, uint8_t count=6);
        void begin(void);
        float readTemperature(bool S=false);
        float convertCtoF(float);
        float readHumidity(void);

};

#endif
