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
    /* Init the class for one relay driven by a PCF component
     * i2cAddress : the address of the PCF component
     * PCFPinNb : the number of the output pin of the PCF component
     */
    RelayI2CDriver(uint8_t i2cAddress, uint8_t PCFPinNb);

    /* turn on the realy */
    void on();
    /* turn off the relay */
    void off();
    /* Return 1 if the relay is on, 0 if it is not */
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