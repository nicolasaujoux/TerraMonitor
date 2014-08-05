
#include "DS18B20TempSensor.h"


DS18B20TempSensorThread::DS18B20TempSensorThread(uint8_t _address, uint8_t _pin )
{
    /* init the pin to read from */
    oneWirePin = _pin;
    DS18B20Address = _address;
    resetMinMax();
    ds = new OneWire(oneWirePin);

    if (!ds->search(addr)) 
    { // Recherche un module 1-Wire
        ds->reset_search();    // Réinitialise la recherche de module
        // return false;         // Retourne une erreur
    }

    if (OneWire::crc8(addr, 7) != addr[7]) // Vérifie que l'adresse a été correctement reçue
    {
        // return false;                        // Si le message est corrompu on retourne une erreur
    }

    if (addr[0] != DS18B20Address) // Vérifie qu'il s'agit bien d'un DS18B20
    {
        // return false;         // Si ce n'est pas le cas on retourne une erreur
    }
}

void DS18B20TempSensorThread::run()
{
    uint8_t data[9];

    ds->reset();             // On reset le bus 1-Wire
    ds->select(addr);        // On sélectionne le DS18B20

    ds->write(0x44, 1);      // On lance une prise de mesure de température
    delay(800);             // Et on attend la fin de la mesure

    ds->reset();             // On reset le bus 1-Wire
    ds->select(addr);        // On sélectionne le DS18B20
    ds->write(0xBE);         // On envoie une demande de lecture du scratchpad

    for (byte i = 0; i < 9; i++) // On lit le scratchpad
    data[i] = ds->read();       // Et on stock les octets reçus

    // Calcul de la température en degré Celsius
    temp = ((data[1] << 8) | data[0]) * 0.0625; 

    computeMinMax();
    runned();
}

void DS18B20TempSensorThread::computeMinMax()
{
    if (temp > maxValue)
    {
        maxValue = temp;
    }
    if (temp < minValue)
    {
        minValue = temp;
    }
}

void DS18B20TempSensorThread::resetMinMax()
{
    minValue = 100;
    maxValue = 0;
}

/*
 * EOF
 */
