/*
 * HumidityController.h
 */

#ifndef HUMIDITY_CONTROLLER_H
#define HUMIDITY_CONTROLLER_H

#include "RelayI2CDriver.h"
#include "SHT10Sensor.h"
#include "FansController.h"

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
    HumidityController(RelayI2CDriver* relayCommand, SHT10SensorThread* humiditySensor, FansController* fans);
    
    /* Get the information saved in the EEPROM and set the humidity alarms 
     * The clock need to be set before calling it */ 
    uint8_t initAlarms ();

    /* Not used at the moment */
    void setMode (controlMode_t mode);
    /* set an Alarm with the explicit parameters */
    uint8_t setAlarm (uint8_t index, uint8_t hour, uint8_t min, uint8_t sec, uint8_t duration);

    /* Disable a specific alarm */
    uint8_t disableAlarm (uint8_t index);
    /* Disable all fogging alarm in the system */
    uint8_t disableAllAlarms ();

    /* return 1 if the system is currently fogging, 0 if not */
    uint8_t getIsFogging() const {return *pIsFogging;}

    /* Set the timers for the activation of the fans which will prevent steam on glass :
     * delay : time between the end of the fogging and the start of the fans
     * timer : duration of the fans */
    void setAntiSteamFanTimers(uint8_t delay, uint16_t timer);
    /* Getters of the two parameters above */
    uint8_t getAntiSteamFanDelay() const {return *pAntiSteamFanDelay;}
    uint16_t getAntiSteamFanTimer() const {return *pAntiSteamFanTimer;}

protected:
    void readEepromAlarm (uint8_t index, HumidityAlarmParameters_t* params);
    void writeEepromAlarm (uint8_t index, HumidityAlarmParameters_t* params);
    static void fogAlarmStart();
    static void fogAlarmStop();
    static void startAntiSteamFan();
    uint8_t createAlarm (uint8_t index);

    controlMode_t mode;
    uint8_t* pIsFogging;

    uint16_t* pAntiSteamFanTimer;
    uint8_t* pAntiSteamFanDelay;
};

#endif /* HUMIDITY_CONTROLLER_H */
/*
 * EOF
 */