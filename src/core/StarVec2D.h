#ifndef STARVEC2D_H
#define STARVEC2D_H
#include "StariskExport.h"

class STARISK_API StarVec2D
{
    public:
    
    float x = 0.0f, y = 0.0f;
    
    StarVec2D();
    StarVec2D(float x, float y);
    StarVec2D(float bothValue);
    ~StarVec2D();

    StarVec2D& operator+=(const StarVec2D& other); 
    StarVec2D operator+(const StarVec2D& other) const;

    StarVec2D& operator-=(const StarVec2D& other);
    StarVec2D operator-(const StarVec2D& other) const;

    StarVec2D& operator*=(const StarVec2D& other);
    StarVec2D operator*(const StarVec2D& other) const;
    
    StarVec2D& operator/=(const StarVec2D& other);
    StarVec2D operator/(const StarVec2D& other) const;


    


};

#endif