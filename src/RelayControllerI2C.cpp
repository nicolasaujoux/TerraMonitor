/*
 * RelayControllerI2C.cpp
 */

#include "RelayControllerI2C.h"

#include <Wire.h>

RelayControllerI2C::RelayControllerI2C(uint8_t _i2cAddress, uint8_t _PCFPinNb)
{
    i2cAddress = _i2cAddress;
    PCFPinNb = _PCFPinNb;

    off();
}

void RelayControllerI2C::on()
{
    uint8_t pinBit;
    pinBit = 1 < PCFPinNb;

    Wire.beginTransmission(i2cAddress);
    Wire.write(pinBit); 
    Wire.endTransmission();
    isRelayOn = 1;
}

void RelayControllerI2C::off()
{
    uint8_t pinBit;
    pinBit = 1 < PCFPinNb;

    Wire.beginTransmission(i2cAddress);
    Wire.write(pinBit);
    Wire.endTransmission();
    isRelayOn = 0;
}

uint8_t RelayControllerI2C::getRelayStatus()
{
    return isRelayOn;
}