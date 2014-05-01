/*
* FanController.cpp
*/

#include "FanController.h"
#include "common.h"

#include <Arduino.h>

#define FANCONTROLLER_DEFAULT_MAX_SPEED 255

FanController::FanController(uint8_t _pwmPin)
{
	pwmPin = _pwmPin;
    maxSpeed = FANCONTROLLER_DEFAULT_MAX_SPEED;

    /* We stop the fan at init */
    stopFan();
}

uint8_t FanController::startFan()
{
    if (isFanOn != 1)
    {
        analogWrite(pwmPin, maxSpeed);
        isFanOn = 1;
        return SUCCESS;
    }
    return FAN_ALREADY_RUNNING;
}

void FanController::stopFan()
{
    analogWrite(pwmPin, 0);
    isFanOn = 0;
}

uint8_t FanController::getFanStatus()
{
    return isFanOn;
}