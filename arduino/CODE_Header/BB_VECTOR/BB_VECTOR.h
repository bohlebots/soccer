// -------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////
/**
*    BOHLEBOTS VECTOR HEADER-FILE
*   
*    Version: 0.1.0
*    Date: 7. Mai 2023
*     
*    Users Guide:
*       this header can do everything of the basic vector math 
*       for which the function names and other comments should be 
*       enough IMPORTANT is that this vector header is optimized 
*       for the BohleBots therefore their way of thinking about 
*       directions is provided here
*                  ^ x
*                  |           
*                  |     @1
*                  |   /
*                  | /
*        ----------|----------> y
*                / |
*              /   |
*             @2   |
*                  *
*        the diagram shows that this header provides a view
*        in which the axis from front to back of the robot is 
*        the x-axis and the axis from left to right is the y-axis
*        therefore also @1 has an angle value of around +45° 
*        and @2 of around -135°
*/
//////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------------
#ifndef BB_VECTOR_H
#define BB_VECTOR_H
//////////////////////////////////////////////////////////////////////
/*
*   INCLUDES / DEPENDENCIES
*/
//////////////////////////////////////////////////////////////////////
#include <Arduino.h>
#include <Math.h>
//////////////////////////////////////////////////////////////////////
/*
*   DECLARATIONS
*/
//////////////////////////////////////////////////////////////////////
/*
*   BB_VECTOR
*/
class BB_VECTOR {
  public:
    /*
    *   PUBLIC FUNCTIONS
    */
    /************************************************************
    *
    *   CONSTRUCTORS
    *
    ************************************************************/
    BB_VECTOR();
    BB_VECTOR(const double& a, const double& b);
    /************************************************************
    *
    *   GETTER FUNCTIONS
    *
    ************************************************************/
    double      getX() const;
    double      getY() const;
    double  getAngle() const;
    double getRadius() const;
    /************************************************************
    *
    *   CLONING
    *
    ************************************************************/
    BB_VECTOR clone() const;
    static BB_VECTOR clone(const BB_VECTOR& v);
    /************************************************************
    *
    *   OPERATORS
    *
    ************************************************************/
    void      operator +=(const BB_VECTOR& v);
    void      operator -=(const BB_VECTOR& v);
    void      operator *=(const double&    d);
    BB_VECTOR operator  +(const BB_VECTOR& v);
    BB_VECTOR operator  -(const BB_VECTOR& v);
    BB_VECTOR operator  *(const double&    d);
    double    operator  *(const BB_VECTOR& v);
    /************************************************************
    *
    *   ADDITION intern
    *
    ************************************************************/
    void add(const BB_VECTOR& v);
    /************************************************************
    *
    *   ADDITION extern
    *
    ************************************************************/
    static BB_VECTOR add(const BB_VECTOR& v1, const BB_VECTOR& v2);
    /************************************************************
    *
    *   SUBTRACTION intern
    *
    ************************************************************/
    void subtract(const BB_VECTOR& v);
    /************************************************************
    *
    *   SUBTRACTION extern
    *
    ************************************************************/
    static BB_VECTOR subtract(const BB_VECTOR& v1, const BB_VECTOR& v2);
    /************************************************************
    *
    *   MULTIPLICATION intern
    *
    ************************************************************/
    void multiply(const double& d);
    /************************************************************
    *
    *   MULTIPLICATION extern
    *
    ************************************************************/
    static BB_VECTOR multiply(const BB_VECTOR& v,const double& d);
    static double dotProduct(const BB_VECTOR& v1, const BB_VECTOR& v2);
    /************************************************************
    *
    *   SCALING intern
    *
    ************************************************************/
    void scale_XY(const double& a, const double& b);
    /************************************************************
    *
    *   SCALING extern
    *
    ************************************************************/
    static BB_VECTOR scale_XY(const BB_VECTOR& v, const double& a, const double& b);
    /************************************************************
    *
    *   ROTATION intern
    *
    ************************************************************/
    void rotate(const double& angle_rad);
    /************************************************************
    *
    *   ROTATION extern
    *
    ************************************************************/
    static BB_VECTOR rotate(const BB_VECTOR& z, const double& angle_rad);
    /************************************************************
    *
    *   ANGLES
    *
    ************************************************************/
    double angleOffsetTo(const BB_VECTOR& v) const;
    static double angleOffsetBetween(const BB_VECTOR& v1, const BB_VECTOR& v2);
    /************************************************************
    *
    *   SECTORS
    *
    ************************************************************/
    bool isBetween(const BB_VECTOR& v1, const BB_VECTOR& v2) const;
    static bool vectorIsInSector(const BB_VECTOR& v, const BB_VECTOR& a, const BB_VECTOR& b);
    /************************************************************
    *
    *   DEBUG
    *
    ************************************************************/
    String debugString() const;
  /*
  *  PRIVATE IMPLEMENTATIONS
  */
  private: 
    double _x, _y, _angle, _radius;
    void update_XY();
    void update_PhiRad();
};
#endif // BB_VECTOR_H
