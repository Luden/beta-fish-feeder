#include <Blinkenlight.h>
#include <Servo.h>
#include "Button2.h"

Blinkenlight _statusLed(PC13, true);
Button2 _button;
Servo _servo;

const short _stagesCount = 16;
unsigned short _currentStage = 7;

const int _minAngle = 0;
const int _maxAngle = 170;
const int _anglePerStage = 13;
const long _millisInDay = 86400000;
const long _lastStageIncrementTime = 0;
short _angles[] = {12, 12, 12, 12, 13, 13, 14, 14, 15, 12, 12, 12, 12, 12, 12, 12}; // fine-tuning to warped and poorly modelled plastics 

void setup()
{
    _servo.attach(PB12);
    _button.begin(PA0);
    _button.setTapHandler(handleTap);
    _statusLed.setting.ending_ms = 5000;
    UpdateLedStage();
    UpdateServoStage();
}

void loop()
{
    _button.loop();
    _statusLed.update();
}

void handleTap(Button2& button) 
{
    if (button.wasPressedFor() < 50)
        return; 
    _currentStage++;
    if (_currentStage >= _stagesCount)
        _currentStage = 0;
    UpdateLedStage();
    UpdateServoStage();
}

void UpdateLedStage()
{
    _statusLed.pattern(_currentStage + 1);
}

void UpdateServoStage()
{
    int totalAngle = _minAngle;
    for (int i = 0; i < _currentStage; i++)
        totalAngle += _angles[i];
    _servo.write(totalAngle);
}

