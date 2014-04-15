/*
* FanController.cpp
*/

#include "FanController.h"

#include <Arduino.h>

#define FANCONTROLLER_DEFAULT_MAX_SPEED 255

FanController::FanController(int _pwmPin)
{
	pwmPin = _pwmPin;
    maxSpeed = FANCONTROLLER_DEFAULT_MAX_SPEED;

    /* We stop the fan at init */
    stopFan();
}

void FanController::startFan()
{
    analogWrite(pwmPin, maxSpeed);
    isFanOn = 1;
}

void FanController::stopFan()
{
    analogWrite(pwmPin, 0);
    isFanOn = 0;
}

bool FanController::getFanStatus()
{
    return isFanOn;
}