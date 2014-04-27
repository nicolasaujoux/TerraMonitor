/*
*   HumidityController.cpp
*/

#include "HumidityController.h"

#include <EEPROM.h>


/* Global variable to be accessible from TimeAlarms library 
* probably not the best option but nothing else comes to mind right now ...
*/
static RelayControllerI2C* relayCommand;
static HumidityControllerAlarm_t alarms[HUMIDITY_CONTROLLER_MAX_NB_ALARMS];
static SHT10SensorThread* humiditySensor;

HumidityController::HumidityController(RelayControllerI2C* _relayCommand, SHT10SensorThread* _humiditySensor)
{
    relayCommand = _relayCommand;
    humiditySensor = _humiditySensor;

    /* set all alarmsId of the array to 255 (alarm free) */
    for (uint8_t i = 0; i < HUMIDITY_CONTROLLER_MAX_NB_ALARMS; ++i)
    {
        alarms[i].id = 255;
    }

    initAlarms();
}

void HumidityController::setMode (controlMode_t _mode)
{
    mode = _mode;
}

uint8_t HumidityController::setAlarm (uint8_t _index, uint8_t _hour, uint8_t _min, uint8_t _sec, uint8_t _duration)
{
    /* if an alarm already exist for this _index, we delete it */
    if (Alarm.isAllocated(alarms[_index].id))
    {
        Alarm.free(alarms[_index].id);
    }

    alarms[_index].params.hour = _hour;
    alarms[_index].params.min = _min;
    alarms[_index].params.sec = _sec;
    alarms[_index].params.duration = _duration;
    alarms[_index].params.isOn = 1;

    if (createAlarm(_index) != SUCCESS)
    {
        return ERROR;
    }

    /* Saves Alarm in EEPROM */
    writeEepromAlarm(_index, &(alarms[_index].params));

    return SUCCESS;
}

uint8_t HumidityController::disableAlarm (uint8_t _index)
{
    /* check if an alarm exist */
    if (Alarm.isAllocated(alarms[_index].id))
    {
        Alarm.disable(alarms[_index].id);
        alarms[_index].params.isOn = 0;
        /* Saves Alarm in EEPROM */
        writeEepromAlarm(_index, &(alarms[_index].params));
        return SUCCESS;
    }

    return ERROR;
}

uint8_t HumidityController::disableAllAlarms ()
{
    for (uint8_t i = 0; i < HUMIDITY_CONTROLLER_MAX_NB_ALARMS; ++i)
    {
        disableAlarm(i);
    }

    return SUCCESS;
}

/**********************************************
 Private functions 
 **********************************************/

void HumidityController::fogAlarmStop()
{
    // relayCommand->off();
    Serial.println("stop");
}

void HumidityController::fogAlarmStart()
{
    uint8_t i;
    AlarmID_t tmpId; 
    tmpId = Alarm.getTriggeredAlarmId();

    /* Gets the triggered alarm */
    for (i = 0; i < HUMIDITY_CONTROLLER_MAX_NB_ALARMS; ++i)
    {
        if (alarms[i].id == tmpId)
        {
            break;
        }
    }
    // /* Switch on the relay */
    // relayCommand->on();

    Serial.println("start");
    /* Set up a timer to stop the relay */
    Alarm.timerOnce(alarms[i].params.duration, fogAlarmStop);
}

void HumidityController::initAlarms ()
{
    for (uint8_t i = 0; i < HUMIDITY_CONTROLLER_MAX_NB_ALARMS; ++i)
    {
        readEepromAlarm(i, &(alarms[i].params));
        if (createAlarm(i) != SUCCESS)
        {
            return ERROR;
        }
    }
}

void HumidityController::readEepromAlarm (uint8_t _index, AlarmParameters_t* _pParams)
{
    uint8_t* pRead;

    pRead = (uint8_t*)_pParams;

    for (uint8_t i = 0; i < (sizeof(AlarmParameters_t)/sizeof(uint8_t)); ++i)
    {
        *(pRead + i) = EEPROM.read(EEPROM_HUMIDITY_CONTROLLER_START_ADDR + 
            (_index * sizeof(AlarmParameters_t)) + i);
    }
}

void HumidityController::writeEepromAlarm (uint8_t _index, AlarmParameters_t* _pParams)
{
    uint8_t* pWrite;
    
    pWrite = (uint8_t*)_pParams;

    for (uint8_t i = 0; i < (sizeof(AlarmParameters_t)/sizeof(uint8_t)); ++i)
    {
        EEPROM.write(EEPROM_HUMIDITY_CONTROLLER_START_ADDR + (_index * sizeof(AlarmParameters_t)) + i,
            *(pWrite + i));
    }
}

uint8_t HumidityController::createAlarm (uint8_t _index)
{
    /* check alarm parameters */
    if (alarms[_index].params.hour >= 0 && alarms[_index].params.hour < 24 &&
        alarms[_index].params.min >= 0 && alarms[_index].params.min < 60 &&
        alarms[_index].params.sec >= 0 && alarms[_index].params.sec < 60 &&
        alarms[_index].params.isOn >= 0 && alarms[_index].params.isOn < 2)
    {
        alarms[_index].id = Alarm.alarmRepeat(alarms[_index].params.hour, alarms[_index].params.min, 
                alarms[_index].params.sec, fogAlarmStart);
        if (alarms[_index].id == dtINVALID_ALARM_ID)
        {
            return ERROR;
        }
        if (alarms[_index].params.isOn == 1)
        {
            Alarm.enable(alarms[_index].id);
        }
        else
        {
            Alarm.disable(alarms[_index].id);
        }
    }
    return SUCCESS;
}

