/*
 * FanController.h
 */

#ifndef FAN_CONTROLLER_H
#define FAN_CONTROLLER_H

class FanController
{
public:
    FanController(int pwmPin);

    void startFan();
    void stopFan();
    bool getFanStatus();

protected:
    int pwmPin;
    int maxSpeed;
    bool isFanOn;
};

#endif /* FAN_CONTROLLER_H */
/*
 * EOF
 */