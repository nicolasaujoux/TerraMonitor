/*
*   HumidityController.cpp
*/

#include "HumidityController.h"

#include <EEPROM.h>
#include "common.h"


/* Global variable to be accessible from TimeAlarms library 
* probably not the best option but nothing else comes to mind right now ...
*/
static RelayI2CDriver* humidityRelayCommand;
static HumidityControllerAlarm_t alarms[HUMIDITY_CONTROLLER_MAX_NB_ALARMS];
static SHT10SensorThread* humiditySensor;
static FansController* fansController;

static uint8_t isFogging;
static uint16_t antiSteamFanTimer;
static uint8_t antiSteamFanDelay;

#define ANTI_STEAM_TIMER_DEFAULT 90
#define ANTI_STEAM_DELAY_DEFAULT 30

HumidityController::HumidityController(RelayI2CDriver* _humidityRelayCommand, 
    SHT10SensorThread* _humiditySensor, FansController* _fans)
{
    humidityRelayCommand = _humidityRelayCommand;
    humiditySensor = _humiditySensor;
    fansController = _fans;

    /* set all alarmsId of the array to 255 (alarm free) */
    for (uint8_t i = 0; i < HUMIDITY_CONTROLLER_MAX_NB_ALARMS; ++i)
    {
        alarms[i].id = 255;
    }

    pIsFogging = &isFogging;
    isFogging = 0;

    pAntiSteamFanTimer = &antiSteamFanTimer;
    antiSteamFanTimer = ANTI_STEAM_TIMER_DEFAULT;
    pAntiSteamFanDelay = &antiSteamFanDelay;
    antiSteamFanDelay = ANTI_STEAM_DELAY_DEFAULT;

    initAlarms();
}

uint8_t HumidityController::initAlarms ()
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

void HumidityController::setAntiSteamFanTimers(uint8_t delay, uint16_t timer)
{
    *pAntiSteamFanTimer = timer;
    *pAntiSteamFanDelay = delay;
}

/**********************************************
 Private functions 
 **********************************************/

void HumidityController::startAntiSteamFan()
{
    fansController->airInForSeconds(antiSteamFanTimer);
}

void HumidityController::fogAlarmStop()
{
    AlarmID_t ret;

    humidityRelayCommand->off();
    isFogging = 0;
    Alarm.timerOnce(antiSteamFanDelay, startAntiSteamFan);
}

void HumidityController::fogAlarmStart()
{
    uint8_t i;
    AlarmID_t ret;
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
    /* Disable Fans for the fogging period */
    fansController->disableFansForSeconds(alarms[i].params.duration);

    /* Switch on the relay */
    humidityRelayCommand->on();
    isFogging = 1;

    /* Set up a timer to stop the relay */
    ret = Alarm.timerOnce(alarms[i].params.duration, fogAlarmStop);
    if (ret == dtINVALID_ALARM_ID)
    {
        humidityRelayCommand->off();
        isFogging = 0;
    }
}

void HumidityController::readEepromAlarm (uint8_t _index, HumidityAlarmParameters_t* _pParams)
{
    uint8_t* pRead;

    pRead = (uint8_t*)_pParams;

    for (uint8_t i = 0; i < (sizeof(HumidityAlarmParameters_t)/sizeof(uint8_t)); ++i)
    {
        *(pRead + i) = EEPROM.read(EEPROM_HUMIDITY_CONTROLLER_START_ADDR + 
            (_index * sizeof(HumidityAlarmParameters_t)) + i);
    }
}

void HumidityController::writeEepromAlarm (uint8_t _index, HumidityAlarmParameters_t* _pParams)
{
    uint8_t* pWrite;
    
    pWrite = (uint8_t*)_pParams;

    for (uint8_t i = 0; i < (sizeof(HumidityAlarmParameters_t)/sizeof(uint8_t)); ++i)
    {
        EEPROM.write(EEPROM_HUMIDITY_CONTROLLER_START_ADDR + (_index * sizeof(HumidityAlarmParameters_t)) + i,
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

/*
* EOF
*/