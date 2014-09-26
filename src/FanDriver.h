/*
 * FanDriver.h
 */

#ifndef FAN_DRIVER_H
#define FAN_DRIVER_H

#include <inttypes.h>

class FanDriver
{
public:
    /* Creator of the FanDriver class.
     * The parameter is the number of the PWM pin driving the fan
     */
    FanDriver(uint8_t pwmPin);

    /* Start the fan */
    uint8_t startFan(uint8_t speed=0);
    /* Stop the fan */
    void stopFan();
    /* Return 1 if the fan is on, 0 if off */
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