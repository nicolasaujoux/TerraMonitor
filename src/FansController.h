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

    uint8_t getIsExtracting() const {return *pIsExtracting;}
    uint8_t getIsAiring() const {return *pIsAiring;}

protected:
    uint8_t* pIsExtracting; 
    uint8_t* pIsAiring;
};

#endif /* FANSCONTROLLER_H */
/*
 * EOF
 */