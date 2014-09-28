/*
* FansController.cpp
*/

#include "FansController.h"
#include <TimeAlarms.h>
#include "common.h"

static FanDriver* extractFan;
static FanDriver* frontFan;
static FanDriver* sideFan;

static uint8_t isExtracting;
static uint8_t isAiring;
static uint8_t fansEnableStatus;

FansController::FansController(FanDriver* _extractFan, FanDriver* _frontFan, FanDriver* _sideFan)
{
    extractFan = _extractFan;
    frontFan = _frontFan;
    sideFan = _sideFan;

    pIsExtracting = &isExtracting;
    isExtracting = 0;
    pIsAiring = &isAiring;
    isAiring = 0;
    pFansEnableStatus = &fansEnableStatus;
    fansEnableStatus = 1;
}

void FansController::stopExtract()
{
    extractFan->stopFan();
    isExtracting = 0;
}

void FansController::stopAirIn()
{
    frontFan->stopFan();
    sideFan->stopFan();
    isAiring = 0;
}

void FansController::fansEnable()
{
    fansEnableStatus = 1;
}

uint8_t FansController::extractForSeconds(uint16_t timerSeconds, uint8_t speed)
{
    AlarmID_t ret;

    if (fansEnableStatus == 0)
    {
        return FAN_DISABLED;
    }

    if (extractFan->getFanStatus() != 1)
    {
        extractFan->startFan(speed);
    }
    else
    {
        return FAN_ALREADY_RUNNING;
    }
    /* Set up a timer to stop the fan */
    ret = Alarm.timerOnce(timerSeconds, stopExtract);
    if (ret == dtINVALID_ALARM_ID)
    {
        extractFan->stopFan();
        return ERROR;
    }
    isExtracting = 1;
    return SUCCESS;
}

uint8_t FansController::airInForSeconds(uint16_t timerSeconds, uint8_t speed)
{
    AlarmID_t ret;

    if (fansEnableStatus == 0)
    {
        return FAN_DISABLED;
    }
    
    if (frontFan->getFanStatus() != 1 && sideFan->getFanStatus() != 1)
    {
        frontFan->startFan(speed);
        sideFan->startFan(speed);
    }
    else
    {
        return FAN_ALREADY_RUNNING;
    }
    /* Set up a timer to stop the fan */
    ret = Alarm.timerOnce(timerSeconds, stopAirIn);
    if (ret == dtINVALID_ALARM_ID)
    {
        frontFan->stopFan();
        sideFan->stopFan();
        return ERROR;
    }
    isAiring = 1;
    return SUCCESS;
}

uint8_t FansController::disableFansForSeconds(uint16_t time)
{
    AlarmID_t ret;
    /* Stop all fans */
    stopExtract();
    stopAirIn();

    /* Disable fans */
    fansEnableStatus = 0;
    /* Set up a timer to stop the fan */
    ret = Alarm.timerOnce(time, fansEnable);
    if (ret == dtINVALID_ALARM_ID)
    {
        fansEnableStatus = 1;
        return ERROR;
    }
    return SUCCESS;
}