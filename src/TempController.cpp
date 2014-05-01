/*
*   TempController.cpp
*/

#include "TempController.h"

#include <EEPROM.h>


/* Global variable to be accessible from TimeAlarms library 
* probably not the best option but nothing else comes to mind right now ...
*/
static RelayControllerI2C* pHeaterRelayCommand;
static FanController* pHeaterFan1;
static FanController* pHeaterFan2;
static FanController* pExtractFan;






/*
* EOF
*/