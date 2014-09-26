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
static DS18B20TempSensorThread* pTempSensorHigh;
static DS18B20TempSensorThread* pTempSensorLow;
static LightController* pLightController;

TempController::TempController(RelayI2CDriver* _pHeaterRelayCommand, FansController* _pFans, 
    DS18B20TempSensorThread* _pTempSensorHigh, DS18B20TempSensorThread* _pTempSensorLow, 
    LightController* _pLightController)
{
    pHeaterRelayCommand = _pHeaterRelayCommand;
    pFans = _pFans;
    pTempSensorHigh = _pTempSensorHigh;
    pTempSensorLow = _pTempSensorLow;
    pLightController = _pLightController;
}

void TempController::run()
{
    float tempHigh, tempLow;
    tempHigh = pTempSensorHigh->getTemp();
    tempLow = pTempSensorLow->getTemp();

    if (tempHigh >= 29)
    {
        if (pFans->getIsExtracting() == false)
        {
            pFans->extractForSeconds(60);
        }
    }

    /* If it's night time */
    if (pLightController->getIsLightOn() == false)
    {
        if (pHeaterRelayCommand->getRelayStatus() == false)
        {
            pHeaterRelayCommand->on();    
        }
        if (tempLow < 21)
        {
            if (pFans->getIsAiring() == false)
            {
                pFans->airInForSeconds(200, 175);
            }
        }
        else
        {
            pHeaterRelayCommand->off();
        }
    }
    /* Day time */
    else
    {
        if (pHeaterRelayCommand->getRelayStatus() == true)
        {
            pHeaterRelayCommand->off();    
        }
        if (tempLow < 21)
        {
            if (pFans->getIsAiring() == false)
            {
                pFans->airInForSeconds(60, 175);
            }
        }
    }

    runned();
}



/*
* EOF
*/