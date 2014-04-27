/*
* common.h
*/

#ifndef COMMON_H
#define COMMON_H


/*****************************************************
*                  Errors define
*****************************************************/

#define SUCCESS 0
#define ERROR 1


#define EEPROM_HUMIDITY_CONTROLLER_START_ADDR 0
#define EEPROM_HUMIDITY_CONTROLLER_SIZE HUMIDITY_CONTROLLER_MAX_NB_ALARMS * sizeof(AlarmParameters_t)

#endif /* COMMON_H */
/*
 * EOF
 */