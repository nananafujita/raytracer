#include "Vec3.h"

#include <cmath>

const Vec3 Vec3::origin(0.0f, 0.0f, 0.0f);

Vec3::Vec3(float x_, float y_, float z_) 
: x(x_), y(y_), z(z_)
{
}

Vec3 operator+(const Vec3& a, const Vec3& b)
{
    return Vec3( a.x + b.x, a.y + b.y, a.z + b.z );
}

Vec3 operator-(const Vec3& a, const Vec3& b)
{
    return Vec3( a.x - b.x, a.y - b.y, a.z - b.z );
}

Vec3 Vec3::cross(const Vec3& a, const Vec3& b) { 
    return Vec3 ( 
        a.y * b.z - a.z * b.y, 
        a.z * b.x - a.x * b.z, 
        a.x * b.y - a.y * b.x 
    ); 
}

float Vec3::dot(const Vec3& a, const Vec3& b) { 
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

// returns angle in radians
float Vec3::angleBetween(const Vec3& a, const Vec3& b)
{
    return acos(dot(a, b) / (magnitude(a) * magnitude(b)));
}

float Vec3::magnitude(const Vec3& a)
{
    return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
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