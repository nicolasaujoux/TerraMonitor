/*
 * FanDriver.h
 */

#ifndef FAN_DRIVER_H
#define FAN_DRIVER_H

#include <inttypes.h>

class FanDriver
{
public:
    FanDriver(uint8_t pwmPin);

    uint8_t startFan();
    void stopFan();
    uint8_t getFanStatus();

protected:
    uint8_t pwmPin;
    uint8_t maxSpeed;
    uint8_t isFanOn;
};

#endif /* FAN_DRIVER_H */
/*
 * EOF
 */