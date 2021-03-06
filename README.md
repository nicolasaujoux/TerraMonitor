TerraMonitor
============

This is an Arduino Project used to monitor temperature, humidity and light in
a terrarium.

Libraries
---------

All the libraries necessary to compile the project are in the lib folder.
Here is a list of the libraries and a link to their project.
* Arduino Makefile : https://github.com/sudar/Arduino-Makefile
* DS1307RTC : in the Time library
* SHT10 humidity and temperature sensor
  : https://github.com/Trefex/arduino-airquality . This one seems to work better
  than the official.
* LCD i2c screen : http://www.dfrobot.com/wiki/index.php/I2C/TWI_LCD1602_Module_(Gadgeteer_Compatible)_(SKU:_DFR0063).
* ArduinoThread : https://github.com/ivanseidel/ArduinoThread
* TimerOne (for interrupt) : http://playground.arduino.cc/code/timer1

Makefile Usage
--------------
* Use `make` to compile the project
* `make upload` to upload it to the arduino
* `make monitor` to dispay serial output (screen package necessary)