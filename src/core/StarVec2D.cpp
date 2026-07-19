#include "StarVec2D.h"


StarVec2D::StarVec2D(){
    x = 0.0f;
    y = 0.0f;
}

StarVec2D::StarVec2D(float x, float y) : x(x), y(y) {}

StarVec2D::StarVec2D(float bothValue) : x(bothValue), y(bothValue) {}
StarVec2D::~StarVec2D(){}


StarVec2D StarVec2D::operator+(const StarVec2D& other) const
{
    return StarVec2D(x + other.x, y + other.y);
}

StarVec2D& StarVec2D::operator+=(const StarVec2D& other) 
{
    *this = *this + other;
    return *this;
}

StarVec2D StarVec2D::operator+=(const float val)
{
    return StarVec2D(x + val, y + val);
}

StarVec2D StarVec2D::operator+(const float val)
{
    return *this += val;
}


StarVec2D StarVec2D::operator-(const StarVec2D& other) const
{
    return StarVec2D(x - other.x, y - other.y);
}

StarVec2D& StarVec2D::operator-=(const StarVec2D& other)
{
    *this = *this - other;
    return *this;
}

StarVec2D StarVec2D::operator-=(const float val)
{
    return StarVec2D(x - val, y - val);
}

StarVec2D StarVec2D::operator-(const float val)
{
    return *this -= val;
}

StarVec2D StarVec2D::operator*(const StarVec2D& other) const
{
    return StarVec2D(x * other.x, y * other.y);
}

StarVec2D& StarVec2D::operator*=(const StarVec2D& other)
{
    *this = *this * other;
    return *this;
}

StarVec2D StarVec2D::operator*=(const float val)
{
    return StarVec2D(x * val, y * val);

}

StarVec2D StarVec2D::operator*(const float val)
{
    return *this *= val;
}

StarVec2D StarVec2D::operator/(const StarVec2D& other) const 
{
    return StarVec2D(x / other.x, y / other.y);
}

StarVec2D& StarVec2D::operator/=(const StarVec2D& other) 
{
    *this = *this / other;
    return *this;
}

StarVec2D StarVec2D::operator/=(const float val)
{
    return StarVec2D(x / val, y / val);
  
}

StarVec2D StarVec2D::operator/(const float val)
{
    return *this /= val;
}
