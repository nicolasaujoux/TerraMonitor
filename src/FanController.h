/*
 * FanController.h
 */

#ifndef FAN_CONTROLLER_H
#define FAN_CONTROLLER_H

#include <inttypes.h>

class FanController
{
public:
    FanController(uint8_t pwmPin);

    uint8_t startFan();
    void stopFan();
    uint8_t getFanStatus();

protected:
    uint8_t pwmPin;
    uint8_t maxSpeed;
    uint8_t isFanOn;
};

#endif /* FAN_CONTROLLER_H */
/*
 * EOF
 */