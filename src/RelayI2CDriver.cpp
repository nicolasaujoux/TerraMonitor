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

    // off();
}

void RelayI2CDriver::on()
{
    Wire.beginTransmission(i2cAddress);
    Serial.println(PCFOutputState);
    PCFOutputState &= ~(1 << PCFPinNb); //this doesn't work ??
    Serial.println(PCFOutputState);
    Wire.write(PCFOutputState);
    Wire.endTransmission();

    isRelayOn = 0;
}

void RelayI2CDriver::off()
{
    Wire.beginTransmission(i2cAddress);
    Serial.println(PCFOutputState);
    PCFOutputState |= (1 << PCFPinNb);  
    Serial.println(PCFOutputState);  
    Wire.write(PCFOutputState); 
    Wire.endTransmission();

    isRelayOn = 1;
}

uint8_t RelayI2CDriver::getRelayStatus()
{
    return isRelayOn;
}