/*
 * RelayControllerI2C.h
 * It uses a PCF controller
 */

#ifndef RELAY_CONTROLLER_I2C_H
#define RELAY_CONTROLLER_I2C_H

#include <stdint.h>

class RelayControllerI2C
{
public:
    RelayControllerI2C(uint8_t i2cAddress, uint8_t PCFPinNb);

    void on();
    void off();
    uint8_t getRelayStatus();

protected:
    uint8_t i2cAddress;
    uint8_t PCFPinNb;
    uint8_t isRelayOn;
};

#endif /* RELAY_CONTROLLER_I2C_H */
/*
 * EOF
 */