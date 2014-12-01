/*
 * TempController.h
*/

#ifndef TEMP_CONTROLLER_H
#define TEMP_CONTROLLER_H

#include "RelayI2CDriver.h"
#include "DS18B20TempSensor.h"
#include "FansController.h"
#include "LightController.h"

typedef struct 
{
    uint8_t maxUpperTemp;
    uint8_t minDayLowerTemp;
    uint8_t minNightLowerTemp;
    uint8_t minNightUpperTemp;
} TempControlParameters_t;

class TempController: public Thread
{
public:
    TempController(RelayI2CDriver* pHeaterRelayCommand, FansController* pFans, 
    	DS18B20TempSensorThread* pTempSensorHigh, DS18B20TempSensorThread* pTempSensorLow, LightController* pLight);
    /* run is automatically called by the Thread process */
    void run();

    void setMaxUpperTemp (uint8_t temp);
    void setMinDayLowerTemp (uint8_t temp);
    void setMinNightLowerTemp (uint8_t temp);
    void setMinNightUpperTemp (uint8_t temp);

protected:
	void readEepromParam (TempControlParameters_t* _pParams);
	void writeEepromParam (TempControlParameters_t* _pParams);
    float (*pGetTemp)();
    TempControlParameters_t tempParameters;

};

#endif /* TEMP_CONTROLLER_H */
/*
 * EOF
 */