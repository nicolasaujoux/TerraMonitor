/*
* common.h
*/

#ifndef COMMON_H
#define COMMON_H

#include "HumidityController.h"
#include "LightController.h"
#include "TempController.h"

/*****************************************************
*                  Errors define
*****************************************************/

#define SUCCESS 0
#define ERROR 1

/* FAN errors */
#define FAN_ALREADY_RUNNING 10
#define FAN_DISABLED		11

/*****************************************************
*                  EEPROM MAPPING
*****************************************************/

#define EEPROM_HUMIDITY_CONTROLLER_START_ADDR 0
#define EEPROM_HUMIDITY_CONTROLLER_SIZE (HUMIDITY_CONTROLLER_MAX_NB_ALARMS * sizeof(HumidityAlarmParameters_t))

#define EEPROM_LIGHT_CONTROLLER_START_ADDR (EEPROM_HUMIDITY_CONTROLLER_START_ADDR + EEPROM_HUMIDITY_CONTROLLER_SIZE)
#define EEPROM_LIGHT_CONTROLLER_SIZE (LIGHT_CONTROLLER_MAX_NB_ALARMS * sizeof(LightAlarmParameters_t))

#define EEPROM_TEMP_CONTROLLER_START_ADDR (EEPROM_LIGHT_CONTROLLER_START_ADDR + EEPROM_LIGHT_CONTROLLER_SIZE)
#define EEPROM_TEMP_CONTROLLER_SIZE (sizeof(TempControlParameters_t))

#endif /* COMMON_H */
/*
 * EOF
 */