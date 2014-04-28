/*
* common.h
*/

#ifndef COMMON_H
#define COMMON_H

#include "HumidityController.h"
#include "LightController.h"

/*****************************************************
*                  Errors define
*****************************************************/

#define SUCCESS 0
#define ERROR 1

/*****************************************************
*                  EEPROM MAPPING
*****************************************************/

#define EEPROM_HUMIDITY_CONTROLLER_START_ADDR 0
#define EEPROM_HUMIDITY_CONTROLLER_SIZE (HUMIDITY_CONTROLLER_MAX_NB_ALARMS * sizeof(HumidityAlarmParameters_t))

#define EEPROM_LIGHT_CONTROLLER_START_ADDR (EEPROM_HUMIDITY_CONTROLLER_START_ADDR + EEPROM_HUMIDITY_CONTROLLER_SIZE)
#define EEPROM_LIGHT_CONTROLLER_SIZE (LIGHT_CONTROLLER_MAX_NB_ALARMS * sizeof(LightAlarmParameters_t))

#endif /* COMMON_H */
/*
 * EOF
 */