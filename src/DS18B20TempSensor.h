/*
 * DS18B20TempSensor.h
 */

#ifndef DS18B20_TEMP_SENSOR_H
#define DS18B20_TEMP_SENSOR_H

#include "Thread.h"
#include <OneWire.h>
 

// Create a new Class, called DS18B20TempSensorThread, that inherits from Thread
class DS18B20TempSensorThread: public Thread
{
public:
    DS18B20TempSensorThread(uint8_t address, uint8_t pin);
    /* run is automatically called by the Thread process 
     * it updates the temperature and humidity therefor, calling getAverageValue retuns the
     * value from the last call to run() */
    void run();
    float getTemp() const {return temp;}
    float getMinValue() const {return minValue;}
    float getMaxValue() const {return maxValue;}
    void resetMinMax();

protected:
    uint8_t DS18B20Address, oneWirePin;
    uint8_t addr[8];
    OneWire* ds;
    float temp, minValue, maxValue;

    void computeMinMax();
};

#endif /* TEMP_SENSOR_H */
/*
 * EOF
 */