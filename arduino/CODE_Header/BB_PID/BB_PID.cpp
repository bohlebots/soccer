// -------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////
/**
*      BOHLEBOTS PID CONTROLLER CPP-FILE
*     
*      Version: 0.4.0
*      Date: 25. Mai 2023
*/
//////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------------

#include "BB_PID.h"

////////////////////////////////////////////////////////////////
//--- CONSTRUCTOR --------------------------------------------//
////////////////////////////////////////////////////////////////

BB_PID::BB_PID() 
  : _setpoint(0.0),
    _Kp(0.0), _Ki(0.0), _Kd(0.0),
    _pTerm(0.0), _iTerm(0.0), _dTerm(0.0),
    _lastError(0.0),
    _minOutput(0.0), _maxOutput(1.0),
    _interval(1), _dt(0),
    _state(ON), _direction(DIRECT),
    _output(0.0), _transferFunctionPtr([](const double& d) { return d; }) {
}



////////////////////////////////////////////////////////////////
//--- SETTER FUNCTIONS ---------------------------------------//
////////////////////////////////////////////////////////////////

// setSetpoint
int BB_PID::setSetpoint(const double& setpoint) {
  // Execute
  this->_setpoint = setpoint;
  this->_iTerm = 0;
  return 0;
}

// setTunings
int BB_PID::setTunings(const double& kp, const double& ki, const double& kd) {
  // Check for Valid Input
  if (kp < 0 || ki < 0 || kd < 0) {
    return -1;
  }
  // Execute
  this->_Kp =   kp;
  this->_Ki =   ki / this->_interval;
  this->_Kd = - kd * this->_interval;
  if (this->_direction == REVERSE) {
    this->_Kp *= -1;
    this->_Ki *= -1;
    this->_Kd *= -1;
  }
  return 0;
}

// setInterval
int BB_PID::setInterval(const int& millis) {
  // Check for valid Input
  if (millis <= 0) {
    return -1;
  }
  // Execute
  double ratio = (double)millis / (double)this->_interval;
  this->_Ki /= ratio;
  this->_Kd *= ratio;
  this->_interval = millis;
  return 0;
}

// setOutputLimits
int BB_PID::setOutputLimits(const double& minOut, const double& maxOut) {
  // Check for valid Input
  if (minOut > maxOut) {
    return -1;
  }
  // Execute
  this->_minOutput = minOut;
  this->_maxOutput = maxOut;
  return 0;
}

// setState
int BB_PID::setState(const BB_PID_STATE& state) {
  // Execute
  this->_state = state;
  if (this->_state == OFF) {
    this->_iTerm = 0.0;
  }
  return 0;
}

// setDirection
int BB_PID::setDirection(const BB_PID_DIRECTION& direction) {
  // Execute
  if(this->_direction != direction) { 
    this->_direction = direction;
    this->_Kp *= -1;
    this->_Ki *= -1;
    this->_Kd *= -1;
  }
  return 0;
}

// setTransferFunction
int BB_PID::setTransferFunction(double (*functionPtr)(const double& d)) {
  // Execute
  this->_transferFunctionPtr = functionPtr;
  return 0;
}



////////////////////////////////////////////////////////////////
//--- COMPUTATION --------------------------------------------//
////////////////////////////////////////////////////////////////

// compute
int BB_PID::compute(const double& input) {
  // Check for valid Input
  if (this->_dt < this->_interval) {
    return -1;
  }
  if (this->_state == PAUSE) {
    return -2;
  }
  if (this->_state == OFF) {
    return -3;
  }
  // Execute
  double error = input - this->_setpoint;
  // Reset iTerm if error crosses 0
  if ((error > 0 && this->_lastError < 0) || (error < 0 && this->_lastError > 0)) {
    this->_iTerm = 0.0;
  }
  double dError = error - this->_lastError;
  this->_pTerm  = this->_Kp * error;
  this->_iTerm += this->_Ki * error * this->_interval;
  this->_dTerm  = this->_Kd * dError / this->_interval;
  this->_output = this->_pTerm + this->_iTerm - this->_dTerm;
  // Maybe needed corrections
  if (this->_output > this->_maxOutput) {
    this->_iTerm -= this->_output - this->_maxOutput;
    this->_output = this->_maxOutput;
  }
  if (this->_output < this->_minOutput) {
    this->_iTerm += this->_minOutput - this->_output;
    this->_output = this->_minOutput;
  }
  // Safe new values for next calculation
  this->_lastError  = error;
  this->_dt = 0;
  return 0;
}



////////////////////////////////////////////////////////////////
//--- GETTER FUNCTIONS ---------------------------------------//
////////////////////////////////////////////////////////////////

// getRawOutput
double BB_PID::getRawOutput() const {
  // Execute
  return this->_output;
}

// getTransferedOutput
double BB_PID::getTransferedOutput() const {
  // Execute
  return this->_transferFunctionPtr(this->_output);
}



////////////////////////////////////////////////////////////////
//--- DEBUG FUNCTIONS ---------------------------------------//
////////////////////////////////////////////////////////////////

// debugPrint
void BB_PID::debugPrint(const String& label) const {
  // String for verbal represenattion of the direction
  String sDirection = "undefined";
  switch(this->_direction) {
    case DIRECT:  sDirection = "DIRECT";  break;
    case REVERSE: sDirection = "REVERSE"; break;
  }
  // String for verbal represenattion of the state
  String sState     = "undefined";
  switch(this->_state) {
    case OFF:   sState = "OFF";   break;
    case PAUSE: sState = "PAUSE"; break;
    case ON:    sState = "ON";    break;
  }
  // Debug print
  Serial.println("+-----------------+--------------------------------------");
  Serial.println("| PID DEBUG START |" + label );
  Serial.println("+-----------------+");
  Serial.println("   [ Settings ]    ");
  Serial.println("        setpoint:  " + String(this->_setpoint ));
  Serial.println("        Kp:        " + String(this->_Kp       ));
  Serial.println("        Ki:        " + String(this->_Ki       ));
  Serial.println("        Kd:        " + String(this->_Kd       ));
  Serial.println("        minOutput: " + String(this->_minOutput));
  Serial.println("        maxOutput: " + String(this->_maxOutput));
  Serial.println("        interval:  " + String(this->_interval ));
  Serial.println("        direction: " + sDirection              );
  Serial.println("                   ");
  Serial.println("   [ State ]       ");
  Serial.println("        state:     " + sState                  );
  Serial.println("        pTerm:     " + String(this->_pTerm    ));
  Serial.println("        iTerm:     " + String(this->_iTerm    ));
  Serial.println("        dTerm:     " + String(this->_dTerm    ));
  Serial.println("        dt:        " + String(this->_dt       ));
  Serial.println("        lastError: " + String(this->_lastError));
  Serial.println("        output:    " + String(this->_output   ));
  Serial.println("                   ");
  Serial.println("+-----------------+");
  Serial.println("|  PID DEBUG END  |" + label );
  Serial.println("+-----------------+--------------------------------------");
}
