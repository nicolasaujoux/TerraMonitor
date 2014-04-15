/*
 * TempSensor.h
 */

#ifndef TEMP_SENSOR_H
#define TEMP_SENSOR_H

#include "Thread.h"

#define TEMPSENSOR_HISTORIC_MAX_NB_VALUE 60

// Create a new Class, called SensorThread, that inherits from Thread
class TempSensorThread: public Thread
{
public:
    TempSensorThread(int pin);
    void run();
    float getAverageValue() const {return averageValue;}
    float getMinValue() const {return minValue;}
    float getMaxValue() const {return maxValue;}
    void resetMinMax();

protected:
    int analogPin;
    float averageValue, minValue, maxValue;
    float historicValue[TEMPSENSOR_HISTORIC_MAX_NB_VALUE];
    float multCoeff;
    int nbIter;
    
    void computeAverage();
    void computeMinMax();
};

#endif /* TEMP_SENSOR_H */
/*
 * EOF
 */