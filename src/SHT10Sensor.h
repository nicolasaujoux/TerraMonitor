/*
 * SHT10Sensor.h
 */

#ifndef SHT10_SENSOR_H
#define SHT10_SENSOR_H

#include "Thread.h"
#include "SHT1x.h"
// Create a new Class, called SensorThread, that inherits from Thread
class SHT10SensorThread: public Thread
{
public:
    SHT10SensorThread(int clock, int data);
    ~SHT10SensorThread();
    void run();
    float getTemp() const {return temp;}
    float getMinTemp() const {return minTemp;}
    float getMaxTemp() const {return maxTemp;}
    void resetMinMaxTemp();
    float getHumidity() const {return humidity;}
    float getMinHumidity() const {return minHumidity;}
    float getMaxHumidity() const {return maxHumidity;}
    void resetMinMaxHumidity();

protected:
    SHT1x* sensor;
    int clockPin, dataPin;
    float temp, minTemp, maxTemp;
    float humidity, minHumidity, maxHumidity;

    void computeMinMaxTemp();
    void computeMinMaxHumidity();
};

#endif /* SHT10_SENSOR_H */
/*
 * EOF
 */