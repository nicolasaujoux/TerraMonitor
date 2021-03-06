
#include <Arduino.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Time.h>
#include <DS1307RTC.h>
#include <TimeAlarms.h>
#include "ThreadController.h"
#include "TimerOne.h"

#include "common.h"

#include "DS18B20TempSensor.h"
#include "SHT10Sensor.h"
#include "FanDriver.h"
#include "RelayI2CDriver.h"
#include "HumidityController.h"
#include "LightController.h"
#include "FansController.h"
#include "TempController.h"

#define DS18B20 0x28     // Adresse 1-Wire du DS18B20
#define LOW_SENSOR_PIN 12 // Broche utilisée pour le bus 1-Wire
#define HIGH_SENSOR_PIN 13 // Broche utilisée pour le bus 1-Wire

#define SHT10_DATA_PIN 10
#define SHT10_CLOCK_PIN 11

#define FAN_EXTRACT_PIN 9
#define FAN_HEATER_SIDE_PIN 5 //old one
#define FAN_HEATER_FRONT_PIN 6 //new one
// #define FAN_EXTRACT_PIN 5
// #define FAN_HEATER_SIDE_PIN 6 //old one
// #define FAN_HEATER_FRONT_PIN 9 //new one

#define RELAY_PCF_I2C_ADDRESS 0x38
#define RELAY_PCF_FOG_PINNB 0
#define RELAY_PCF_HEAT_PINNB 1
#define RELAY_PCF_LIGHT_PINNB 2

// Instantiate a new ThreadController
ThreadController controller = ThreadController();

/* Sensors */
// TempSensorThread lowSensor(LOW_SENSOR_PIN);
DS18B20TempSensorThread lowSensor(DS18B20, LOW_SENSOR_PIN);
DS18B20TempSensorThread highSensor(DS18B20, HIGH_SENSOR_PIN);
SHT10SensorThread humidTempSensor(SHT10_CLOCK_PIN, SHT10_DATA_PIN);

/* Screen init */
LiquidCrystal_I2C lcd(0x20,20,4);

/* Fan init */
FanDriver extractFan(FAN_EXTRACT_PIN);
FanDriver sideFan(FAN_HEATER_SIDE_PIN);
FanDriver frontFan(FAN_HEATER_FRONT_PIN);

FansController fans(&extractFan, &frontFan, &sideFan);

/* Relay init */
RelayI2CDriver fogRelay(RELAY_PCF_I2C_ADDRESS, RELAY_PCF_FOG_PINNB);
RelayI2CDriver lightRelay(RELAY_PCF_I2C_ADDRESS, RELAY_PCF_LIGHT_PINNB);
RelayI2CDriver heatRelay(RELAY_PCF_I2C_ADDRESS, RELAY_PCF_HEAT_PINNB);

/* Humidity controller */
HumidityController humidityController(&fogRelay, &humidTempSensor, &fans);
/* Light Controller */
LightController lightController(&lightRelay);
/* Temperature controller */
TempController tempController(&heatRelay, &fans, &highSensor, &lowSensor, &lightController);

void digitalClockDisplay(time_t time);
void printDigits(int digits, char separator);


// This is the callback for the Timer
void timerCallback()
{
    // Serial.print("Interrupt");
}

// functions to be called when an alarm triggers:
// void MorningAlarm(){
//   fogRelay.on();
//   delay(30000);
//   fogRelay.off();
// }

void setup()
{

    tmElements_t currentTime;

    currentTime.Second = 0;
    currentTime.Minute = 35;
    currentTime.Hour =18;
    currentTime.Wday = 5;
    currentTime.Day = 17;
    currentTime.Month = 4;
    currentTime.Year = 44;

    // RTC.set(makeTime(currentTime));

    Serial.begin(9600);
    Serial.println("TerraMonitor starting ...");
    Wire.begin(); 

    // analogReference(INTERNAL);

    Serial.print("EEPROM HumidityController size occupied (in bytes) : ");
    Serial.println(EEPROM_HUMIDITY_CONTROLLER_SIZE);

    setSyncProvider(RTC.get);   // the function to get the time from the RTC    
    if(timeStatus()!= timeSet) 
        Serial.println("Unable to sync with the RTC");
    else
        Serial.println("RTC has set the system time");

    /* Controller Alarm init */
    humidityController.initAlarms();
    lightController.initAlarms();

    /* read the sensor every s */
    lowSensor.setInterval(5000);
    highSensor.setInterval(5000);
    /* read the sensor every 10s */
    humidTempSensor.setInterval(10000); 
    /* run temp controller every s */
    tempController.setInterval(1000);

    /* add the thread to the controller */
    controller.add(&lowSensor);
    controller.add(&highSensor);
    controller.add(&humidTempSensor);
    controller.add(&tempController);
    
    Timer1.initialize(100000);
    Timer1.attachInterrupt(timerCallback);

    // Alarms
    //Alarm.alarmRepeat(12,00,0, MorningAlarm);  // 8:30am every day
    // Alarm.alarmRepeat(14,48,0, MorningAlarm);  // 8:30am every day
    //Alarm.alarmRepeat(17,30,0, MorningAlarm);  // 8:30am every day
    //Alarm.alarmRepeat(22,15,0, MorningAlarm);  // 8:30am every day

    humidityController.disableAllAlarms();
    humidityController.setAlarm(0, 12, 0, 0, 30);
    humidityController.setAlarm(1, 14, 0, 0, 15);
    humidityController.setAlarm(2, 16, 0, 0, 15);
    humidityController.setAlarm(3, 20, 0, 0, 15);
    humidityController.setAlarm(4, 22, 15, 0, 20);

    // humidityController.setAlarm(4, 16, 10, 0, 10);

    lightController.setStartTime(11,30,0);
    lightController.setStopTime(23,30,0);

    tempController.setMaxUpperTemp(29);
    tempController.setMinDayLowerTemp(21);
    tempController.setMinNightLowerTemp(20);
    tempController.setMinNightUpperTemp(21);

    // extractFan.startFan();
    // sideFan.startFan();
    // frontFan.startFan();

    lcd.init();                      // initialize the lcd 
 
    // Print a message to the LCD.
    lcd.backlight();

}

void loop()
{
    /* Runs the non RT controller */
    controller.run();

    // Serial.print(" Temperature milieu: ");
    // Serial.print(humidTempSensor.getTemp());
    // Serial.print(" Humidite milieu: ");
    // Serial.print(humidTempSensor.getHumidity());
    // Serial.print(" Temperature Bas: ");
    // Serial.println(lowSensor.getAverageValue());
    // Serial.println();

    // lcd.cursor();
    // lcd.clear();
    lcd.setCursor(7,0);
    digitalClockDisplay(now());
    lcd.setCursor(0,1);
    lcd.print("Up : ");
    lcd.print(highSensor.getTemp());
    lcd.print((char)223);
    lcd.print('C');
    // lcd.setCursor(0,2);
    // lcd.print("Mid : ");
    // lcd.print(humidTempSensor.getTemp());
    // lcd.print((char)223);
    // lcd.print('C');
    lcd.setCursor(0,2);
    lcd.print("Down : ");
    lcd.print(lowSensor.getTemp());
    lcd.print((char)223);
    lcd.print('C');

    // fogRelay.on();
    // lightRelay.on();
    // // heatRelay.on();
    // delay(5000);
    // fogRelay.off();
    // lightRelay.off();
    // // heatRelay.off();
    lcd.setCursor(0,1);
    Alarm.delay(1000);
}

void digitalClockDisplay(time_t time)
{
    // digital clock display of the time
    printDigits(hour(time), NULL);
    printDigits(minute(time), ':');
    // printDigits(second(time), ':');
}

void printDigits(int digits, char separator)
{
    if (separator != NULL)
    {
        lcd.print(separator);
        Serial.print(separator);
    }
    if(digits < 10)
    {
        lcd.print('0');
        Serial.print('0');
    }
    lcd.print(digits);
    Serial.print(digits);
}

