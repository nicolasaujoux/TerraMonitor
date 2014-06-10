/*
 * RelayI2CDriver.cpp
 */

#include "RelayI2CDriver.h"

#include <Wire.h>
#include <Arduino.h>

uint8_t RelayI2CDriver::PCFOutputState = 0xFF;

RelayI2CDriver::RelayI2CDriver(uint8_t _i2cAddress, uint8_t _PCFPinNb)
{
    i2cAddress = _i2cAddress;
    PCFPinNb = _PCFPinNb;
}

void RelayI2CDriver::on()
{
    Wire.beginTransmission(i2cAddress);
    PCFOutputState &= ~(1 << PCFPinNb);
    Wire.write(PCFOutputState);
    Wire.endTransmission();

    isRelayOn = 0;
}

void RelayI2CDriver::off()
{
    Wire.beginTransmission(i2cAddress);
    PCFOutputState |= (1 << PCFPinNb); 
    Wire.write(PCFOutputState); 
    Wire.endTransmission();

    isRelayOn = 1;
}

uint8_t RelayI2CDriver::getRelayStatus()
{
    return isRelayOn;
}