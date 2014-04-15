/*
 * SHT10Sensor.cpp
 */
#include "SHT10Sensor.h"

SHT10SensorThread::SHT10SensorThread(int _clock, int _data)
{
    /* init the pin to read from */
    clockPin = _clock;
    dataPin = _data;
    
    /* Create sensor instance */
    sensor = new SHT1x(dataPin, clockPin);

    /* init min and max */
    resetMinMaxTemp();
    resetMinMaxHumidity();
}

SHT10SensorThread::~SHT10SensorThread()
{
    free(sensor);
}

void SHT10SensorThread::run()
{
    temp = sensor->retrieveTemperatureC(); 
    computeMinMaxTemp(); 
    humidity = sensor->readHumidity(); 
    computeMinMaxHumidity();

    runned();
}

void SHT10SensorThread::computeMinMaxTemp()
{
    if (temp > maxTemp)
    {
        maxTemp = temp;
    }
    if (temp < minTemp)
    {
        minTemp = temp;
    }
}

void SHT10SensorThread::resetMinMaxTemp()
{
    minTemp = 100;
    maxTemp = 0;
}

void SHT10SensorThread::computeMinMaxHumidity()
{
    if (humidity > maxHumidity)
    {
        maxHumidity = humidity;
    }
    if (humidity < minHumidity)
    {
        minHumidity = humidity;
    }
}

void SHT10SensorThread::resetMinMaxHumidity()
{
    minHumidity = 120;
    maxHumidity = 0;
}
/*
 * EOF
 */
