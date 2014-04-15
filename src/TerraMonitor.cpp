
#include <Arduino.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#include "ThreadController.h"
#include "TimerOne.h"

#include "TempSensor.h"
#include "SHT10Sensor.h"
#include "FanController.h"

#define LOW_SENSOR_PIN 0

#define SHT10_DATA_PIN 10
#define SHT10_CLOCK_PIN 11

#define FAN_EXTRACT_PIN 5
#define FAN_HEATER_SIDE_PIN 6 //old one
#define FAN_HEATER_FRONT_PIN 9 //new one


TempSensorThread lowSensor = TempSensorThread(LOW_SENSOR_PIN);
SHT10SensorThread humidTempSensor = SHT10SensorThread(SHT10_CLOCK_PIN, SHT10_DATA_PIN);

// Instantiate a new ThreadController
ThreadController controller = ThreadController();

/* Screen init */
LiquidCrystal_I2C lcd(0x20,20,4);

/* Fan init */
FanController extratFan(FAN_EXTRACT_PIN);
FanController sideFan(FAN_HEATER_SIDE_PIN);
FanController frontFan(FAN_HEATER_FRONT_PIN);

int debugNbIter = 0;


// This is the callback for the Timer
void timerCallback()
{
	controller.run();
}

void setup()
{
    Serial.begin(9600);

    /* read the sensor every s */
    lowSensor.setInterval(1000);
    /* read the sensor every 10s */
    humidTempSensor.setInterval(10000); 

    /* add the thread to the controller */
    controller.add(&lowSensor);
    controller.add(&humidTempSensor);
    
    // Timer1.initialize(100000);
    // Timer1.attachInterrupt(timerCallback);

    lcd.init();                      // initialize the lcd 
 
    // Print a message to the LCD.
    lcd.backlight();
}

void loop()
{
    controller.run();
    // Serial.print(" Temperature milieu: ");
    // Serial.print(humidTempSensor.getTemp());
    // Serial.print(" Humidite milieu: ");
    // Serial.print(humidTempSensor.getHumidity());
    // Serial.print(" Temperature Bas: ");
    // Serial.println(lowSensor.getAverageValue());

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(humidTempSensor.getTemp());
    lcd.print("Degres C");
    lcd.setCursor(0,1);
    lcd.print(humidTempSensor.getHumidity());
    lcd.print("%");
    lcd.setCursor(0,2);
    lcd.print(lowSensor.getAverageValue());
    lcd.print("Degres C");

    delay(2000);
    debugNbIter++;
}
