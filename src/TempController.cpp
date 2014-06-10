/*
*   TempController.cpp
*/

#include "TempController.h"

#include <EEPROM.h>


/* Global variable to be accessible from TimeAlarms library 
* probably not the best option but nothing else comes to mind right now ...
*/
static RelayI2CDriver* pHeaterRelayCommand;
static FansController* pFans;
static SHT10SensorThread* pTempSensor;

TempController::TempController(RelayI2CDriver* _pHeaterRelayCommand, FansController* _pFans, SHT10SensorThread* _pTempSensor)
{
    pHeaterRelayCommand = _pHeaterRelayCommand;
    pFans = _pFans;
    pTempSensor = _pTempSensor;
}

void TempController::run()
{
    float temp;
    temp = pTempSensor->getTemp();

    if (temp >= 29)
    {
        if (pFans->getIsExtracting() == 0)
        {
            pFans->extractForSeconds(60);
        }
    }
    runned();
}



/*
* EOF
*/