/*
 * FansController.h
 */

#ifndef FANSCONTROLLER_H
#define FANSCONTROLLER_H

#include <inttypes.h>

#include "FanDriver.h"

class FansController
{
public:
    FansController(FanDriver* extractFan, FanDriver* frontFan, FanDriver* sideFan);

    /* Activate the extract fan (it pushes air outside the roof) for "timerSeconds" seconds */
    uint8_t extractForSeconds(uint16_t timerSeconds, uint8_t speed=0);
    /* Activate the fan that push air in the terrarium for "timerSeconds" seconds */
    uint8_t airInForSeconds(uint16_t timerSeconds, uint8_t speed=0);
    
    static void stopExtract();
    static void stopAirIn();
    static void fansEnable();

    uint8_t getIsExtracting() const {return *pIsExtracting;}
    uint8_t getIsAiring() const {return *pIsAiring;}

    /* Disable fans for X seconds, ie : any startFan command has no effect for this period of time */
    uint8_t disableFansForSeconds(uint16_t time);

protected:
    uint8_t* pIsExtracting; 
    uint8_t* pIsAiring;

    uint8_t* pFansEnableStatus;
};

#endif /* FANSCONTROLLER_H */
/*
 * EOF
 */