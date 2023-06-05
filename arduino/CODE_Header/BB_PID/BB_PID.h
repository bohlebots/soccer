// -------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////
/**
*      BOHLEBOTS PID CONTROLLER HEADER-FILE
*     
*      Version: 0.4.0
*      Date: 25. Mai 2023
*       
*      Users Guide:
*         1.) Return value of non-getter functions: 
*               0 = ok, 
*             < 0 = error
*         2.) if you don't know what a transfer function is or 
*             how to use function pointers don't use 
*             setTransferFunction 
*             and use getRawOutput instead of getTransferedFunction
*/
//////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------------
#ifndef BB_PID_H
#define BB_PID_H
//////////////////////////////////////////////////////////////////////
/*
*   INCLUDES / DEPENDENCIES
*/
//////////////////////////////////////////////////////////////////////
#include <elapsedMillis.h>
//////////////////////////////////////////////////////////////////////
/*
*   DECLARATIONS
*/
//////////////////////////////////////////////////////////////////////
/*
*   Setting enums
*/
enum BB_PID_STATE     : int { OFF    = 0, PAUSE, ON }; 
// PAUSE holds iTerm, OFF rests iTerm
enum BB_PID_DIRECTION : int { DIRECT = 0, REVERSE }; 
// DIRECT = higher output -> closer to setpoint, REVERSE = lower output -> closer to setpoint
/*
*   BB_PID
*/
class BB_PID {
  public:
    /*
    *   PUBLIC FUNCTIONS
    */
    /************************************************************
    *
    *   BB_PID
    *
    *   Contructor
    *
    ************************************************************/
    BB_PID();
    /************************************************************
    *
    *   setSetpoint
    *
    *   sets the desired output value to setpoint
    *
    ************************************************************/
    int setSetpoint(const double& setpoint);
    /************************************************************
    *
    *   setTunings
    *
    *   sets the coefficients for P, I and D terms
    *   to the passed values
    *
    ************************************************************/
    int setTunings(const double& kp, const double& ki, const double& kd);
    /************************************************************
    *
    *   setInterval
    *
    *   sets the time interval for the I and D term to millis
    *
    ************************************************************/
    int setInterval(const int& millis);
    /************************************************************
    *
    *   setOutputLimits
    *
    *   sets the output limits to minOut and maxOut
    *
    ************************************************************/
    int setOutputLimits(const double& minOut, const double& maxOut);
    /************************************************************
    *
    *   setState
    *
    *   sets the state to state
    *
    ************************************************************/
    int setState(const BB_PID_STATE& state);
    /************************************************************
    *
    *   setDirection
    *
    *   sets the direction to direction
    *
    ************************************************************/
    int setDirection(const BB_PID_DIRECTION& direction);
    /************************************************************
    *
    *   setTransferFunction
    *
    *   sets the transfer function to functionPtr
    *
    ************************************************************/
    int setTransferFunction(double(*functionPtr)(const double& d));
    /************************************************************
    *
    *   compute
    *
    *   computes the next output
    *     return -1 => zeit intervall noch nicht
    *     return -2 => pid controller in off state
    *     return -3 => pid controller in pause state
    *
    ************************************************************/
    int compute(const double& input);
    /************************************************************
    *
    *   getRawOutput
    *
    *   returns the current raw output
    *
    ************************************************************/
    double getRawOutput() const;
    /************************************************************
    *
    *   getTransferedOutput
    *
    *   returns the current raw output fed through the set 
    *   transfer function
    *
    ************************************************************/
    double getTransferedOutput() const;
    /************************************************************
    *
    *   debugPrint
    *
    *   makes a debug print in the serial monitor
    *   IMPORTANT
    *     make sure the serial monitor is started properly
    *
    ************************************************************/
    void debugPrint(const String& name) const;
  /*
  *  PRIVATE IMPLEMENTATIONS
  */
  private:
    double _setpoint;                  // desired value
    double _Kp, _Ki, _Kd;              // weights for the P, I and D terms in the final PID addition  
    double _pTerm, _iTerm, _dTerm;     
    double _lastError; 
    double _minOutput, _maxOutput;
    unsigned int _interval;   // length of the interval that is used for the calculation of the integral and derivative
                              // [computationInterval] = millis
    elapsedMillis    _dt;        // change in time since the last compute call
    BB_PID_STATE     _state;     // regulator should be on, off or pause
    BB_PID_DIRECTION _direction; // regulator should be in direct or reverse direction
    double           _output;
    double(*_transferFunctionPtr)(const double& d); // pointer to the function that does the conversion from the pid output to the actual regulative value
    // can be as easy as just passing the output through or just mapping it from a range in [-100;100] to [-1;1] but could also be very heavy math
};
#endif // BB_PID_H
