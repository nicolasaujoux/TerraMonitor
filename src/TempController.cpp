/*
*   TempController.cpp
*/

#include "TempController.h"

#include <EEPROM.h>


/* Global variable to be accessible from TimeAlarms library 
* probably not the best option but nothing else comes to mind right now ...
*/
static RelayI2CDriver* pHeaterRelayCommand;
static FanDriver* pHeaterFan1;
static FanDriver* pHeaterFan2;
static FanDriver* pExtractFan;


TempControllerThread::TempControllerThread(RelayI2CDriver* _pHeaterRelayCommand, FanDriver* _pHeaterFan1, 
        FanDriver* _pExtractFan, FanDriver* _pHeaterFan2=0)
{
    pHeaterRelayCommand = _pHeaterRelayCommand;
    pHeaterFan1 = _pHeaterFan1;
    pHeaterFan2 = _pHeaterFan2;
    pExtractFan = _pExtractFan;
}

void TempControllerThread::run()
{
    
}



/*
* EOF
*/