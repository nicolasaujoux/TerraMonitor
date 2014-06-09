/*
 * RelayI2CDriver.h
 * It uses a PCF controller
 */

#ifndef RELAY_I2C_DRIVER_H
#define RELAY_I2C_DRIVER_H

#include <stdint.h>

class RelayI2CDriver
{
public:
    RelayI2CDriver(uint8_t i2cAddress, uint8_t PCFPinNb);

    void on();
    void off();
    uint8_t getRelayStatus();

protected:
    uint8_t i2cAddress;
    uint8_t PCFPinNb;
    uint8_t isRelayOn;

    /* This variable is the image of the PCF outputs 
    * As the read on the PCF seems to not be working, we had to use a static */
    static uint8_t PCFOutputState;
};

#endif /* RELAY_I2C_DRIVER_H */
/*
 * EOF
 */