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

    uint8_t extractForSeconds(uint16_t timerSeconds);
    uint8_t airInForSeconds(uint16_t timerSeconds);
    static void stopExtract();
    static void stopAirIn();

protected:

};

#endif /* FANSCONTROLLER_H */
/*
 * EOF
 */