/*
* FanDriver.cpp
*/

#include "FanDriver.h"
#include "common.h"

#include <Arduino.h>

#define FANDRIVER_DEFAULT_MAX_SPEED 255

FanDriver::FanDriver(uint8_t _pwmPin)
{
	pwmPin = _pwmPin;
    maxSpeed = FANDRIVER_DEFAULT_MAX_SPEED;

    /* We stop the fan at init */
    stopFan();
}

uint8_t FanDriver::startFan(uint8_t _speed)
{
    uint8_t speed;

    if ((_speed == 0) || (_speed > 255))
    {
        speed = maxSpeed;
    }
    else
    {
        speed = _speed;
    }

    if (isFanOn != 1)
    {
        analogWrite(pwmPin, speed);
        isFanOn = 1;
        return SUCCESS;
    }
    return FAN_ALREADY_RUNNING;
}

void FanDriver::stopFan()
{
    analogWrite(pwmPin, 0);
    isFanOn = 0;
}

uint8_t FanDriver::getFanStatus()
{
    return isFanOn;
}