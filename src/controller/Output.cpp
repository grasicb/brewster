#include "Output.h"

Output::Output(String name, uint8_t pin, boolean enablePWM) {
    String loggerName = String("Output "+name);
    char* buff = (char*)malloc(loggerName.length()+1);
    loggerName.toCharArray(buff, loggerName.length()+1);

    _logger = new Logger(buff);
    _logger->trace(loggerName);
    _output = 0;
    _lastOutput = 0;
    _name = name;
    _pin = pin;
    _isPWM = enablePWM;
    _pidOn = false;
    digitalWrite(_pin, 0);
}

void Output::setOutput(uint8_t percentage) {
    _logger->info("Setting output to %d%%", percentage);
    if (percentage > 100) {
      percentage = 100;
    }else if (percentage < 0) {
      percentage = 0;
    }

    _pidOn = false;
    _output = percentage * 2.55;
    if (_pid != NULL) {
      _pid->SetMode(MANUAL);
    }
}

void Output::setTargetValue(double target, float* input) {
  _logger->info("Setting target value to %4.1f°", target);

  //If the reference to input is changed, we need to instantiate a new PID
  if (input != _input) {
    if (_pid != NULL) {
      //delete _pid;
      _pid->SetMode(MANUAL);
    }
    _pid = new PID(input, &_output, &_target, 2, 5, 1, DIRECT);
    _pid->SetMode(AUTOMATIC);

  //If just the target value is changed, then we just adapt the target value
  }else{
    _target = target;
    _pid->SetMode(AUTOMATIC);
  }

  //set local variables
  _target = target;
  _input = input;
  _pidOn = true;
}

void Output::changeTargetValue(double target) {
  if (_input == NULL)
    _logger->warn("Changing set value on an object with input not set");
  if (!_pidOn)
    _logger->warn("Changing set value on an object with disabled PID");

  _logger->info("Changing target value to %4.1f°", target);
  _target = target;
}

double Output::getTargetValue() {
  return _target;
}

void Output::process() {
  if (_pidOn) {
    _pid->Compute();
  }

  //If PWM is used on the PIN then setting PWM value, the rest is done by the controller
  if (_isPWM && _lastOutput != _output) {
    analogWrite(_pin, _output);
    _lastOutput = _output;

  //If it is standard digital output, then output needs to be set proportionally
  }if(_lastOutput == _output && (_output == 0 || _output ==255)) {
    return;
  } else {
    while (millis() - _windowStartTime > _timeWindow)
    {
      _windowStartTime += _timeWindow;
    }



    if (_output/255*_timeWindow > millis() - _windowStartTime) {
      pinSetFast(_pin);
      //_logger->trace("%3.0f | %3.0f | %3u | ON", _output, _output/255*_timeWindow, millis() - _windowStartTime);
    } else {
      pinResetFast(_pin);
      //_logger->trace("%3.0f | %3.0f | %3u | OFF", _output, _output/255*_timeWindow, millis() - _windowStartTime);
    }

    _lastOutput = _output;
  }
}

boolean Output::isActive() {
  //_logger->trace("isActive - pidOn:%i, output:%3f, isActive:%i", _pidOn, _output, !(_output == 0 && _pidOn == false));
  if (_output == 0 && _pidOn == false)
    return false;
  else
    return true;
}

boolean Output::isPID() {
  return _pidOn;
}

uint8_t Output::getOutput() {
  return static_cast<uint8_t>(_output / 2.55);
}
