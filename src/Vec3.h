#ifndef VEC3_H
#define VEC3_H

class Vec3 
{
public:
    Vec3(float x_, float y_, float z_);
    float x, y, z;

    void normalize(); 

    friend Vec3 operator+(const Vec3& a, const Vec3& b);
    friend Vec3 operator-(const Vec3& a, const Vec3& b);

    static float angleBetween(const Vec3& a, const Vec3& b);
    static Vec3 cross(const Vec3& a, const Vec3& b);
    static float dot(const Vec3& a, const Vec3& b);
    static float magnitude(const Vec3& a);
    
    static const Vec3 origin;
};

using Point3 = Vec3;

#endif