/*
 * TempController.h
*/

#ifndef TEMP_CONTROLLER_H
#define TEMP_CONTROLLER_H

#include "common.h"
#include "RelayI2CDriver.h"
 #include "FanDriver.h"

#include <TimeAlarms.h>

class TempControllerThread: public Thread
{
public:
    TempControllerThread(RelayI2CDriver* pHeaterRelayCommand, FanDriver* pHeaterFan, 
        FanDriver* pExtractFan, FanDriver* pHeaterFan2);
    void run();

protected:


};

#endif /* TEMP_CONTROLLER_H */
/*
 * EOF
 */