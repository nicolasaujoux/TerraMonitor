
#include "TempSensor.h"

TempSensorThread::TempSensorThread(int _pin)
{
    /* init the pin to read from */
    analogPin = _pin;
    multCoeff = 1.85;
    analogReference(INTERNAL);
    nbIter = 0;

    /* init the average array with 0 */
    for (int i=0; i<TEMPSENSOR_HISTORIC_MAX_NB_VALUE; i++)
    {
        historicValue[i] = 0;
    }
    /* init min and max */
    resetMinMax();
}

void TempSensorThread::run()
{
    float reading;
    float temp;
    /* we read the value and convert it */
    reading = analogRead(analogPin);
    temp = reading / 9.31 * multCoeff;

    /* store it in the historic array and compute average */
    historicValue[nbIter] = temp;
    nbIter++;
    if (nbIter >= TEMPSENSOR_HISTORIC_MAX_NB_VALUE)
    {
        nbIter = 0;
    }

    computeAverage();
    computeMinMax();
    runned();
}

void TempSensorThread::computeAverage()
{
    float tmpAverage;
    int diviser;

    diviser = TEMPSENSOR_HISTORIC_MAX_NB_VALUE;
    tmpAverage = 0;
    for (int i = 0; i < TEMPSENSOR_HISTORIC_MAX_NB_VALUE; i++)
    {
        tmpAverage += historicValue[i];
        if (historicValue[i] == 0)
        {
            diviser--;
        }
    }
    tmpAverage /= diviser;

    /* update of the class value */
    averageValue = tmpAverage;
}

void TempSensorThread::computeMinMax()
{
    if (averageValue > maxValue)
    {
        maxValue = averageValue;
    }
    if (averageValue < minValue)
    {
        minValue = averageValue;
    }
}

void TempSensorThread::resetMinMax()
{
    minValue = 100;
    maxValue = 0;
}

/*
 * EOF
 */
