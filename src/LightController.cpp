/*
 *   LightController.cpp
*/

#include "LightController.h"

#include <EEPROM.h>

#define LIGHT_CONTROLLER_START_INDEX 0
#define LIGHT_CONTROLLER_STOP_INDEX 1

/* Global variable to be accessible from TimeAlarms library 
* probably not the best option but nothing else comes to mind right now ...
*/
static RelayI2CDriver* lightRelayCommand;

static uint8_t isLightOn;

LightController::LightController(RelayI2CDriver* _lightRelayCommand)
{
    lightRelayCommand = _lightRelayCommand;
    pIsLightOn = &isLightOn;
    isLightOn = 0;

    initAlarms();
}

uint8_t LightController::initAlarms ()
{
    uint32_t currentTime, startTime, stopTime;

    for (uint8_t i = 0; i < LIGHT_CONTROLLER_MAX_NB_ALARMS; ++i)
    {
        readEepromAlarm(i, &(alarms[i].params));
        if (createAlarm(i) != SUCCESS)
        {
            return ERROR;
        }
    }

    currentTime = hoursToTime_t(hour()) + minutesToTime_t(minute()) + second();
    startTime = hoursToTime_t(alarms[LIGHT_CONTROLLER_START_INDEX].params.hour) + 
        minutesToTime_t(alarms[LIGHT_CONTROLLER_START_INDEX].params.min) + 
        alarms[LIGHT_CONTROLLER_START_INDEX].params.sec;
    stopTime = hoursToTime_t(alarms[LIGHT_CONTROLLER_STOP_INDEX].params.hour) + 
        minutesToTime_t(alarms[LIGHT_CONTROLLER_STOP_INDEX].params.min) + 
        alarms[LIGHT_CONTROLLER_STOP_INDEX].params.sec;

    /* Check if the light needs to be on or off */
    if ((currentTime >= startTime) && (currentTime < stopTime))
    {
        lightRelayCommand->on();
        isLightOn = 1;
        return SUCCESS;
    }

    lightRelayCommand->off();
    isLightOn = 0;

    return SUCCESS;
}

uint8_t LightController::setStartTime(uint8_t _hour, uint8_t _min, uint8_t _sec)
{
    alarms[LIGHT_CONTROLLER_START_INDEX].params.hour = _hour;
    alarms[LIGHT_CONTROLLER_START_INDEX].params.min = _min;
    alarms[LIGHT_CONTROLLER_START_INDEX].params.sec = _sec;

    if (createAlarm(LIGHT_CONTROLLER_START_INDEX) != SUCCESS)
    {
        return ERROR;
    }

    /* Saves Alarm in EEPROM */
    writeEepromAlarm(LIGHT_CONTROLLER_START_INDEX, &(alarms[LIGHT_CONTROLLER_START_INDEX].params));
}

uint8_t LightController::setStopTime(uint8_t _hour, uint8_t _min, uint8_t _sec)
{
    alarms[LIGHT_CONTROLLER_STOP_INDEX].params.hour = _hour;
    alarms[LIGHT_CONTROLLER_STOP_INDEX].params.min = _min;
    alarms[LIGHT_CONTROLLER_STOP_INDEX].params.sec = _sec;

    if (createAlarm(LIGHT_CONTROLLER_STOP_INDEX) != SUCCESS)
    {
        return ERROR;
    }

    /* Saves Alarm in EEPROM */
    writeEepromAlarm(LIGHT_CONTROLLER_STOP_INDEX, &(alarms[LIGHT_CONTROLLER_STOP_INDEX].params));
}

/**********************************************
 Private functions 
 **********************************************/

void LightController::lightAlarmStop()
{
    lightRelayCommand->off();
    isLightOn = 0;
}

void LightController::lightAlarmStart()
{
    lightRelayCommand->on();
    isLightOn = 1;
}

void LightController::readEepromAlarm (uint8_t _index, LightAlarmParameters_t* _pParams)
{
    uint8_t* pRead;

    pRead = (uint8_t*)_pParams;

    for (uint8_t i = 0; i < (sizeof(LightAlarmParameters_t)/sizeof(uint8_t)); ++i)
    {
        *(pRead + i) = EEPROM.read(EEPROM_LIGHT_CONTROLLER_START_ADDR + 
            (_index * sizeof(LightAlarmParameters_t)) + i);
    }
}

void LightController::writeEepromAlarm (uint8_t _index, LightAlarmParameters_t* _pParams)
{
    uint8_t* pWrite;
    
    pWrite = (uint8_t*)_pParams;

    for (uint8_t i = 0; i < (sizeof(LightAlarmParameters_t)/sizeof(uint8_t)); ++i)
    {
        EEPROM.write(EEPROM_LIGHT_CONTROLLER_START_ADDR + (_index * sizeof(LightAlarmParameters_t)) + i,
            *(pWrite + i));
    }
}

uint8_t LightController::createAlarm (uint8_t _index)
{
    /* check alarm parameters */
    if (alarms[_index].params.hour >= 0 && alarms[_index].params.hour < 24 &&
        alarms[_index].params.min >= 0 && alarms[_index].params.min < 60 &&
        alarms[_index].params.sec >= 0 && alarms[_index].params.sec < 60)
    {
        /* Start light time */
        if (_index == LIGHT_CONTROLLER_START_INDEX)
        {
            alarms[_index].id = Alarm.alarmRepeat(alarms[_index].params.hour, alarms[_index].params.min, 
                    alarms[_index].params.sec, lightAlarmStart);
            if (alarms[_index].id == dtINVALID_ALARM_ID)
            {
                return ERROR;
            }
        }
        /* Stop light time */
        else if (_index == LIGHT_CONTROLLER_STOP_INDEX)
        {
            alarms[_index].id = Alarm.alarmRepeat(alarms[_index].params.hour, alarms[_index].params.min, 
                    alarms[_index].params.sec, lightAlarmStop);
            if (alarms[_index].id == dtINVALID_ALARM_ID)
            {
                return ERROR;
            }
        }
        else 
        {
            return ERROR;
        }
    }
    return SUCCESS;
}