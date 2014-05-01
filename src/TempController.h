/*
 * TempController.h
*/

#ifndef TEMP_CONTROLLER_H
#define TEMP_CONTROLLER_H

#include "common.h"
#include "RelayControllerI2C.h"
 #include "FanController.h"

#include <TimeAlarms.h>

class TempController
{
public:
    TempController(RelayControllerI2C* pHeaterRelayCommand, FanController* pHeaterFan, 
        FanController* pExtractFan, FanController* pHeaterFan2 = 0);

protected:


};

#endif /* TEMP_CONTROLLER_H */
/*
 * EOF
 */