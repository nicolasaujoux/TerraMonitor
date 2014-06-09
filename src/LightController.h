/*
 * LightController.h
*/

#ifndef LIGHT_CONTROLLER_H
#define LIGHT_CONTROLLER_H

#include "common.h"
#include "RelayI2CDriver.h"

#include <TimeAlarms.h>

typedef struct 
{
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
} LightAlarmParameters_t;

typedef struct 
{
    AlarmID_t id;       /* Id given by the TimeAlarms library */
    LightAlarmParameters_t params;
} LightControllerAlarm_t;

#define LIGHT_CONTROLLER_MAX_NB_ALARMS 2

class LightController
{
public:
    LightController(RelayI2CDriver* lightRelayCommand);
    uint8_t initAlarms ();

    uint8_t setStartTime(uint8_t hour, uint8_t min, uint8_t sec);
    uint8_t setStopTime(uint8_t hour, uint8_t min, uint8_t sec);

protected:
    void readEepromAlarm (uint8_t _index, LightAlarmParameters_t* _pParams);
    void writeEepromAlarm (uint8_t _index, LightAlarmParameters_t* _pParams);
    uint8_t createAlarm (uint8_t _index);

    static void lightAlarmStart();
    static void lightAlarmStop();

    LightControllerAlarm_t alarms[LIGHT_CONTROLLER_MAX_NB_ALARMS];
    uint8_t* pIsLightOn;

};

#endif /* LIGHT_CONTROLLER_H */
/*
 * EOF
 */