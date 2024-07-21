#include <Blinkenlight.h>
#include "Button2.h"

Blinkenlight _statusLed(PC13, true);
Button2 button;

const short _maxStage = 10;
unsigned short _currentStage = 0;

void setup()
{
    button.begin(PA0);
    button.setTapHandler(handleTap);
    _statusLed.setting.ending_ms = 5000;
    _statusLed.on();
}

void loop()
{
    button.loop();
    _statusLed.update();
}

void handleTap(Button2& b) 
{
  if (b.wasPressedFor() > 100) 
  {
        _currentStage++;
        if (_currentStage > _maxStage)
            _currentStage = 0;
        _statusLed.pattern(_currentStage);
  }
}

