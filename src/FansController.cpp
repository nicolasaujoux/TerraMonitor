/*
* FansController.cpp
*/

#include "FansController.h"
#include <TimeAlarms.h>
#include "common.h"

static FanDriver* extractFan;
static FanDriver* frontFan;
static FanDriver* sideFan;

FansController::FansController(FanDriver* _extractFan, FanDriver* _frontFan, FanDriver* _sideFan)
{
    extractFan = _extractFan;
    frontFan = _frontFan;
    sideFan = _sideFan;
}

void FansController::stopExtract()
{
    extractFan->stopFan();
}

void FansController::stopAirIn()
{
    frontFan->stopFan();
    sideFan->stopFan();
}

uint8_t FansController::extractForSeconds(uint16_t timerSeconds)
{
    AlarmID_t ret;
    if (extractFan->getFanStatus() != 1)
    {
        extractFan->startFan();
    }
    else
    {
        return FAN_ALREADY_RUNNING;
    }
    /* Set up a timer to stop the fan */
    ret = Alarm.timerOnce(timerSeconds, stopExtract);
    if (ret == dtINVALID_ALARM_ID)
    {
        return ERROR;
    }
    return SUCCESS;
}

uint8_t FansController::airInForSeconds(uint16_t timerSeconds)
{
    AlarmID_t ret;
    if (frontFan->getFanStatus() != 1 && sideFan->getFanStatus() != 1)
    {
        frontFan->startFan();
        sideFan->startFan();
    }
    else
    {
        return FAN_ALREADY_RUNNING;
    }
    /* Set up a timer to stop the fan */
    ret = Alarm.timerOnce(timerSeconds, stopAirIn);
    if (ret == dtINVALID_ALARM_ID)
    {
        return ERROR;
    }
    return SUCCESS;
}
