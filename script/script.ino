#include <Blinkenlight.h>
#include <Servo.h>
#include "Button2.h"
#include <EEPROM.h>

const short _statusLedPin = 2;
const short _servoPin = 5;
const short _buttonPin = 4;

const long _second = 1000;
const long _minute = 60 * _second;
const long _hour = 60 * _minute;
const long _day = 24 * _hour;
const long _updatePeriod = _day / 2; // adjust period here
const short _angles[] = {11, 11, 12, 12, 13, 13, 14, 15, 13, 13, 13, 12, 12, 12, 12}; // fine-tuning to warped and poorly modelled plastics 
const short _stagesCount = 15;

struct State 
{
  int currentStage;
};

State _state;
Blinkenlight _statusLed(_statusLedPin, true);
Button2 _button;
Servo _servo;
long _lastStageIncrementTime = 0;

void setup()
{
    _servo.attach(5, 544, 2400);
    _button.begin(4);
    _button.setTapHandler(HandleTap);
    _statusLed.setting.ending_ms = _minute;
    EEPROM.begin(4); // esp826 quirk
    LoadState();
    UpdateLedStage();
    UpdateServoStage();
}

void loop()
{
    _button.loop();
    _statusLed.update();
    UpdateStageByTimer();
}

void HandleTap(Button2& button) 
{
    int pressDuration = button.wasPressedFor();
    if (pressDuration < 50)
        return; 
    if (pressDuration > 1000)
        ResetStage();
    else
        IncrementStage();
    _lastStageIncrementTime = millis(); // reset timer on button press
}

void UpdateStageByTimer()
{
    long currentMillis = millis();
    if (currentMillis - _lastStageIncrementTime < _updatePeriod)
        return;
    _lastStageIncrementTime = currentMillis;
    IncrementStage();
}

void IncrementStage()
{
    _state.currentStage++;
    if (_state.currentStage >= _stagesCount)
        _state.currentStage = 0;
    UpdateState();
    SaveState();
}

void ResetStage()
{
    _state.currentStage = 0;
    SaveState();
    UpdateState();
}

void UpdateState()
{
    UpdateLedStage();
    UpdateServoStage();
}

void UpdateLedStage()
{
    _statusLed.pattern(_state.currentStage + 1);
}

void UpdateServoStage()
{
    int totalAngle = 0;
    for (int i = 0; i < _state.currentStage; i++)
        totalAngle += _angles[i];
    _servo.write(totalAngle);
}

void LoadState() 
{
    EEPROM.get(0, _state);
}

void SaveState() 
{
    EEPROM.put(0, _state);
    EEPROM.commit(); // esp826 quirk
}