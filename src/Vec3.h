#ifndef VEC3_H
#define VEC3_H

#include <cmath>

class Vec3 
{
public:
    Vec3(float x_, float y_, float z_);
    float x, y, z;

    void normalize(); 
    
    static const Vec3 origin;
};

using Point3 = Vec3;

inline Vec3 operator+(const Vec3& a, const Vec3& b) 
{
    return Vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline Vec3 operator-(const Vec3& a, const Vec3& b) 
{
    return Vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline Vec3 operator*(const float t, const Vec3& v)
{
    return Vec3(t * v.x, t * v.y, t * v.z);
}

inline float dot(const Vec3& a, const Vec3& b)
{
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

inline float magnitude(const Vec3& a)
{
    return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}

inline float angleBetween(const Vec3& a, const Vec3& b)
{
    return acos(dot(a, b) / (magnitude(a) * magnitude(b)));
}

inline Vec3 cross(const Vec3& a, const Vec3& b)
{
    return Vec3 ( 
        a.y * b.z - a.z * b.y, 
        a.z * b.x - a.x * b.z, 
        a.x * b.y - a.y * b.x 
    ); 
}

#endif