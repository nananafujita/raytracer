#ifndef RAY_H
#define RAY_H

#include "Vec3.h"

class Ray 
{
public: 
    Ray(const Point3& orig, const Vec3& dir) : origin(orig), direction(dir){};
    Point3 at(float t) const { return origin + (t * direction); }
private:
    Point3 origin;
    Vec3 direction;
};

#endif