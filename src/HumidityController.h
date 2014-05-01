/*
 * HumidityController.h
 */

#ifndef HUMIDITY_CONTROLLER_H
#define HUMIDITY_CONTROLLER_H

#include "common.h"

#include "RelayControllerI2C.h"
#include "SHT10Sensor.h"

#include <TimeAlarms.h>

enum controlMode_t
{
    MANU, 
    AUTO_TIME,
    AUTO_HUMIDITY
};

typedef struct 
{
    uint8_t isOn;
    uint8_t duration;  /* Time while the fog relay is on (in s) */
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
} HumidityAlarmParameters_t;

typedef struct 
{
    AlarmID_t id;       /* Id given by the TimeAlarms library */
    HumidityAlarmParameters_t params;
} HumidityControllerAlarm_t;

#define HUMIDITY_CONTROLLER_MAX_NB_ALARMS 6

/*
* This class only drives the fog relay. The humiditySensor must be updated in the main program 
* It uses the timeAlarms library. Therefore, Alarm.delay(x); must be called in the loop() of the 
* program.
*/
class HumidityController
{
public:
    HumidityController(RelayControllerI2C* relayCommand, SHT10SensorThread* humiditySensor);
    uint8_t initAlarms ();

    void setMode (controlMode_t mode);
    uint8_t setAlarm (uint8_t index, uint8_t hour, uint8_t min, uint8_t sec, uint8_t duration);

    uint8_t disableAlarm (uint8_t index);
    uint8_t disableAllAlarms ();

    uint8_t getIsFogging() const {return *pIsFogging;}

protected:
    void readEepromAlarm (uint8_t index, HumidityAlarmParameters_t* params);
    void writeEepromAlarm (uint8_t index, HumidityAlarmParameters_t* params);
    static void fogAlarmStart();
    static void fogAlarmStop();
    uint8_t createAlarm (uint8_t index);

    controlMode_t mode;
    uint8_t* pIsFogging;
};

#endif /* HUMIDITY_CONTROLLER_H */
/*
 * EOF
 */