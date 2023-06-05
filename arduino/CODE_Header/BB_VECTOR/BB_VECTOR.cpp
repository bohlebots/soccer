// -------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////
/**
*    BOHLEBOTS VECTOR CPP-FILE
*   
*    Version: 0.1.0
*    Date: 7. Mai 2023
*/
//////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------------

#include "BB_VECTOR.h"

////////////////////////////////////////////////////////////////
//--- CONSTRUCTOR --------------------------------------------//
////////////////////////////////////////////////////////////////
BB_VECTOR::BB_VECTOR() 
  : _x(0.0), _y(0.0) { 
    this->update_PhiRad(); 
}

BB_VECTOR::BB_VECTOR(const double& a, const double& b) 
  : _x(a), _y(b) { 
    this->update_PhiRad(); 
}

////////////////////////////////////////////////////////////////
//--- GETTER FUNCTIONS ---------------------------------------//
////////////////////////////////////////////////////////////////
double BB_VECTOR::     getX() const { return this->_x;      }
double BB_VECTOR::     getY() const { return this->_y;      }
double BB_VECTOR:: getAngle() const { return this->_angle;  }
double BB_VECTOR::getRadius() const { return this->_radius; }

////////////////////////////////////////////////////////////////
//--- CLONING ------------------------------------------------//
////////////////////////////////////////////////////////////////
BB_VECTOR BB_VECTOR::clone() const { 
  return BB_VECTOR(this->_x, this->_y); 
}
BB_VECTOR BB_VECTOR::clone(const BB_VECTOR& v) {
  return BB_VECTOR(v._x, v._y); 
}
////////////////////////////////////////////////////////////////
//--- OPERATORS ----------------------------------------------//
////////////////////////////////////////////////////////////////
void       BB_VECTOR::operator +=(const BB_VECTOR& v) { this->add(v);                           }
void       BB_VECTOR::operator -=(const BB_VECTOR& v) { this->subtract(v);                      }
void       BB_VECTOR::operator *=(const double&    d) { this->multiply(d);                      }
BB_VECTOR  BB_VECTOR::operator  +(const BB_VECTOR& v) { return BB_VECTOR::       add(*this, v); }
BB_VECTOR  BB_VECTOR::operator  -(const BB_VECTOR& v) { return BB_VECTOR::  subtract(*this, v); }
BB_VECTOR  BB_VECTOR::operator  *(const double&    d) { return BB_VECTOR::  multiply(*this, d); }
double     BB_VECTOR::operator  *(const BB_VECTOR& v) { return BB_VECTOR::dotProduct(*this, v); }

////////////////////////////////////////////////////////////////
//--- ADDITION --- intern ------------------------------------//
////////////////////////////////////////////////////////////////
void BB_VECTOR::add(const BB_VECTOR& v) {
  this->_x += v._x;
  this->_y += v._y;
  this->update_PhiRad();
}

////////////////////////////////////////////////////////////////
//--- ADDITION --- extern ------------------------------------//
////////////////////////////////////////////////////////////////  
BB_VECTOR BB_VECTOR::add(const BB_VECTOR& v1, const BB_VECTOR& v2) { 
  return BB_VECTOR(v1._x + v2._x, v1._y + v2._y);
}

////////////////////////////////////////////////////////////////
//--- SUBTRACTION --- intern ---------------------------------//
////////////////////////////////////////////////////////////////
void BB_VECTOR::subtract(const BB_VECTOR& v) {
  this->_x -= v._x;
  this->_y -= v._y;
  update_PhiRad();
}

////////////////////////////////////////////////////////////////
//--- SUBTRACTION --- extern ---------------------------------//
//////////////////////////////////////////////////////////////// 
BB_VECTOR BB_VECTOR::subtract(const BB_VECTOR& v1, const BB_VECTOR& v2) { 
  return BB_VECTOR(v1._x - v2._x, v1._y - v2._y);
}

////////////////////////////////////////////////////////////////
//--- MULTIPLICATION --- intern ------------------------------//
////////////////////////////////////////////////////////////////
void BB_VECTOR::multiply(const double& d) {
  this->_x *= d;
  this->_y *= d;
  this->update_PhiRad();
}

////////////////////////////////////////////////////////////////
//--- MULTIPLICATION --- extern ------------------------------//
////////////////////////////////////////////////////////////////
BB_VECTOR BB_VECTOR::multiply(const BB_VECTOR& v,const double& d) { 
  return BB_VECTOR(v._x * d, v._y * d);
}
double BB_VECTOR::dotProduct(const BB_VECTOR& v1, const BB_VECTOR& v2) {
    return (v1._x * v2._x) + (v1._y * v2._y);
}

////////////////////////////////////////////////////////////////
//--- SCALING --- intern -------------------------------------//
////////////////////////////////////////////////////////////////
void BB_VECTOR::scale_XY(const double& a, const double& b) {
  this->_x *= a;
  this->_y *= b;
  this->update_PhiRad();
}

////////////////////////////////////////////////////////////////
//--- SCALING --- extern -------------------------------------//
////////////////////////////////////////////////////////////////
BB_VECTOR BB_VECTOR::scale_XY(const BB_VECTOR& v, const double& a, const double& b) {
  return BB_VECTOR(v._x * a, v._y * b);
}  

////////////////////////////////////////////////////////////////
//--- ROTATION --- intern ------------------------------------//
//////////////////////////////////////////////////////////////// 
void BB_VECTOR::rotate(const double& angle_rad) {
  this->_angle += angle_rad;
  this->update_XY();
  this->update_PhiRad();
}

////////////////////////////////////////////////////////////////
//--- ROTATION --- extern ------------------------------------//
///////////////////////////////////////////////////////////////
BB_VECTOR BB_VECTOR::rotate(const BB_VECTOR& v, const double& angle_rad) {
  BB_VECTOR _v = BB_VECTOR::clone(v);
  _v._angle += angle_rad;
  _v.update_XY();
  _v.update_PhiRad();
  return _v;
}

////////////////////////////////////////////////////////////////
//--- ANGLES -------------------------------------------------//
////////////////////////////////////////////////////////////////
double BB_VECTOR::angleOffsetTo(const BB_VECTOR& v) const {
  return acos( BB_VECTOR::dotProduct(*this, v) / (this->_radius * v._radius) );
}

double BB_VECTOR::angleOffsetBetween(const BB_VECTOR& v1, const BB_VECTOR& v2) {
  return acos( BB_VECTOR::dotProduct(v1, v2) / (v1._radius * v2._radius) );
}

////////////////////////////////////////////////////////////////
//--- SECTORS ------------------------------------------------//
////////////////////////////////////////////////////////////////
bool BB_VECTOR::isBetween(const BB_VECTOR& v1, const BB_VECTOR& v2) const {
  int angleThisV1 = (int)(this->angleOffsetTo(v1) * 1000);
  int angleThisV2 = (int)(this->angleOffsetTo(v2) * 1000);
  int angleV1V2   = (int)(BB_VECTOR::angleOffsetBetween(v1, v2) * 1000);
  return ((angleThisV1 + angleThisV2) == angleV1V2);
}

bool BB_VECTOR::vectorIsInSector(const BB_VECTOR& v, const BB_VECTOR& a, const BB_VECTOR& b) {
  int av = (int)(BB_VECTOR::angleOffsetBetween(a, v) * 1000); 
  int vb = (int)(BB_VECTOR::angleOffsetBetween(v, b) * 1000); 
  int ab = (int)(BB_VECTOR::angleOffsetBetween(a, b) * 1000);
  return ((av + vb) == ab);
}

////////////////////////////////////////////////////////////////
//--- DEBUG --------------------------------------------------//
////////////////////////////////////////////////////////////////

String BB_VECTOR::debugString() const {
  String cartesian = "[ x = " + String(this->_x) + ", y = " + String(this->_y) + " ]";
  String polar     = "[ rad = " + String(this->_radius) + ", angle = " + String(this->_angle/PI) + " * PI ]";
  return cartesian + " <=> " + polar;
}

////////////////////////////////////////////////////////////////
//--- PRIVATE ------------------------------------------------//
////////////////////////////////////////////////////////////////

void BB_VECTOR::update_XY() {
  this->_x = this->_radius * cos(this->_angle);
  this->_y = this->_radius * sin(this->_angle);
}

void BB_VECTOR::update_PhiRad() {
  this->_angle  = atan2(this->_y, this->_x);
  this->_radius = sqrt((this->_x * this->_x) + (this->_y * this->_y));
}