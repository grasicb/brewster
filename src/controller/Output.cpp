#include "Output.h"

Output::Output(String name, uint8_t pin, boolean enablePWM) {
    String loggerName = String("Output "+name);
    char* buff = (char*)malloc(loggerName.length()+1);
    loggerName.toCharArray(buff, loggerName.length()+1);

    _logger = new Logger(buff);
    _logger->trace("Initializing output");
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

    triggerChangeEvent();
    _logger->info("Output is set to %i%%", (int)(_output/2.55));
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

  triggerChangeEvent();
  _logger->info("Target value is set to to %4.1f°", _target);
}

void Output::setTargetValue(double target, float* input, int direction) {
  setTargetValue(target, input);
  _pid->SetControllerDirection(direction);
  _logger->info("PID direction set to %i", direction);
}

void Output::changeTargetValue(double target) {
  if (_input == NULL)
    _logger->warn("Changing set value on an object with input not set");
  if (!_pidOn)
    _logger->warn("Changing set value on an object with disabled PID");

  _logger->info("Changing target value to %4.1f°", target);
  _target = target;

  triggerChangeEvent();
  _logger->info("Target value changed to %4.1f°", _target);
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

/*
void Output::SetPidDirection(int direction) {
  _pid->SetControllerDirection(direction);
}
*/

///////////////////////////
// Event handling
///////////////////////////
void Output::addListener(f_outputCB_t function, void* callingObject, int outputIdentifier) {
  OutputListener listener;
  listener.function = function;
  listener.callingObject = callingObject;
  listener.outputIdentifier = outputIdentifier;

  listeners[function] = listener;

  _logger->trace("Added new listener. Total listeners: %i.", listeners.size());
}

void Output::removeListener(f_outputCB_t function) {
  int removed = listeners.erase(function);

  _logger->trace("Removed %i listener(s). Total listeners after removal: %i.", removed, listeners.size());
}

void Output::removeAllListeners() {
  listeners.clear();

  _logger->trace("Removed all listeners.");
}

void Output::triggerChangeEvent() {
  _logger->trace("Triggering change event");
  //Prepare event data
  OutputChangeEvent event;
  event.isActive = isActive();
  event.isPID = isPID();
  if(event.isPID) {
    event.targetValue = getTargetValue();
  }else {
    event.targetValue = (double) getOutput();
  }

  //call events
  for (std::map<f_outputCB_t, OutputListener>::iterator it=listeners.begin(); it!=listeners.end(); ++it)
    it->first(it->second.callingObject, it->second.outputIdentifier, event);
}
