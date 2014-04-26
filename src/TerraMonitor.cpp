
#include <Arduino.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Time.h>
#include <DS1307RTC.h>
#include <TimeAlarms.h>

#include "ThreadController.h"
#include "TimerOne.h"

#include "TempSensor.h"
#include "SHT10Sensor.h"
#include "FanController.h"
#include "RelayControllerI2C.h"

#define LOW_SENSOR_PIN 0

#define SHT10_DATA_PIN 10
#define SHT10_CLOCK_PIN 11

#define FAN_EXTRACT_PIN 5
#define FAN_HEATER_SIDE_PIN 6 //old one
#define FAN_HEATER_FRONT_PIN 9 //new one

#define RELAY_PCF_I2C_ADDRESS 0x38
#define RELAY_PCF_FOG_PINNB 0
#define RELAY_PCF_HEAT_PINNB 1
#define RELAY_PCF_LIGHT_PINNB 2

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

/* Relay init */
RelayControllerI2C fogRelay (RELAY_PCF_I2C_ADDRESS, RELAY_PCF_FOG_PINNB);
RelayControllerI2C lightRelay (RELAY_PCF_I2C_ADDRESS, RELAY_PCF_LIGHT_PINNB);
RelayControllerI2C heatRelay (RELAY_PCF_I2C_ADDRESS, RELAY_PCF_HEAT_PINNB);


void digitalClockDisplay(time_t time);
void printDigits(int digits, char separator);


// This is the callback for the Timer
void timerCallback()
{
	controller.run();
}

// functions to be called when an alarm triggers:
void MorningAlarm(){
  Serial.println("Alarm: - turn lights off");   
  fogRelay.on();
  delay(30000);
  fogRelay.off();
}

void setup()
{
    tmElements_t currentTime;

    currentTime.Second = 0;
    currentTime.Minute = 50;
    currentTime.Hour = 20;
    currentTime.Wday = 5;
    currentTime.Day = 17;
    currentTime.Month = 4;
    currentTime.Year = 44;

    // RTC.set(makeTime(currentTime));

    Serial.begin(9600);
    Wire.begin(); 

    setSyncProvider(RTC.get);   // the function to get the time from the RTC
    if(timeStatus()!= timeSet) 
        Serial.println("Unable to sync with the RTC");
    else
        Serial.println("RTC has set the system time");

    /* read the sensor every s */
    lowSensor.setInterval(1000);
    /* read the sensor every 10s */
    humidTempSensor.setInterval(10000); 

    /* add the thread to the controller */
    controller.add(&lowSensor);
    controller.add(&humidTempSensor);
    
    // Timer1.initialize(100000);
    // Timer1.attachInterrupt(timerCallback);

    // Alarms
    Alarm.alarmRepeat(12,00,0, MorningAlarm);  // 8:30am every day
    Alarm.alarmRepeat(17,30,0, MorningAlarm);  // 8:30am every day
    Alarm.alarmRepeat(22,15,0, MorningAlarm);  // 8:30am every day

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
    Serial.println();

    lcd.clear();
    lcd.setCursor(0,0);
    digitalClockDisplay(now());
    lcd.setCursor(0,1);
    lcd.print(humidTempSensor.getTemp());
    lcd.print("Degres C");
    lcd.setCursor(0,2);
    lcd.print(humidTempSensor.getHumidity());
    lcd.print("%");
    lcd.setCursor(0,3);
    lcd.print(lowSensor.getAverageValue());
    lcd.print("Degres C");

    // fogRelay.on();
    // // lightRelay.on();
    // // heatRelay.on();
    // delay(5000);
    // fogRelay.off();
    // // lightRelay.off();
    // // heatRelay.off();
    delay(1000);
    Alarm.delay(1000);
}

void digitalClockDisplay(time_t time)
{
  // digital clock display of the time
  printDigits(hour(time), NULL);
  printDigits(minute(time), ':');
  printDigits(second(time), ':');
}

void printDigits(int digits, char separator)
{
  if (separator != NULL)
    Serial.print(separator);
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

