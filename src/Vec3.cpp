#include "Vec3.h"

#include <cmath>

const Vec3 Vec3::origin(0.0f, 0.0f, 0.0f);

Vec3::Vec3(float x_, float y_, float z_) 
: x(x_), y(y_), z(z_)
{
}

void Vec3::normalize() 
{
    float len = magnitude(*this);
    if (len > 0){
        x /= len;  
        y /= len;   
        z /= len;
    }
}