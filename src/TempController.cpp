/*
*   TempController.cpp
*/

#include "TempController.h"

#include <EEPROM.h>
#include "common.h"


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

    readEepromParam(&tempParameters);
}

void TempController::run()
{
    float tempHigh, tempLow;
    tempHigh = pTempSensorHigh->getTemp();
    tempLow = pTempSensorLow->getTemp();

    if (tempHigh >= tempParameters.maxUpperTemp)
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
        if (tempLow < tempParameters.minNightLowerTemp)
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
        if (tempLow < tempParameters.minDayLowerTemp)
        {
            if (pFans->getIsAiring() == false)
            {
                pFans->airInForSeconds(60, 175);
            }
        }
    }

    runned();
}

void TempController::setMaxUpperTemp (uint8_t temp)
{
    tempParameters.maxUpperTemp = temp;
    writeEepromParam(&tempParameters);
}

void TempController::setMinDayLowerTemp (uint8_t temp)
{
    tempParameters.minDayLowerTemp = temp;
    writeEepromParam(&tempParameters);
}

void TempController::setMinNightLowerTemp (uint8_t temp)
{
    tempParameters.minNightLowerTemp = temp;
    writeEepromParam(&tempParameters);
}

void TempController::readEepromParam (TempControlParameters_t* _pParams)
{
    uint8_t* pRead;

    pRead = (uint8_t*)_pParams;

    for (uint8_t i = 0; i < (sizeof(TempControlParameters_t)/sizeof(uint8_t)); ++i)
    {
        *(pRead + i) = EEPROM.read(EEPROM_TEMP_CONTROLLER_START_ADDR + 
            sizeof(TempControlParameters_t) + i);
    }
}

void TempController::writeEepromParam (TempControlParameters_t* _pParams)
{
    uint8_t* pWrite;
    
    pWrite = (uint8_t*)_pParams;

    for (uint8_t i = 0; i < (sizeof(TempControlParameters_t)/sizeof(uint8_t)); ++i)
    {
        EEPROM.write(EEPROM_TEMP_CONTROLLER_START_ADDR + sizeof(TempControlParameters_t) + i,
            *(pWrite + i));
    }
}

/*
* EOF
*/