/*
 * TempController.h
*/

#ifndef TEMP_CONTROLLER_H
#define TEMP_CONTROLLER_H

#include "common.h"
#include "RelayI2CDriver.h"
#include "FanDriver.h"
#include "DS18B20TempSensor.h"
#include "LightController.h"

#include <TimeAlarms.h>

class TempController: public Thread
{
public:
    TempController(RelayI2CDriver* pHeaterRelayCommand, FansController* pFans, 
    	DS18B20TempSensorThread* pTempSensorHigh, DS18B20TempSensorThread* pTempSensorLow, LightController* pLight);
    /* run is automatically called by the Thread process */
    void run();

protected:
    float (*pGetTemp)();

};

#endif /* TEMP_CONTROLLER_H */
/*
 * EOF
 */