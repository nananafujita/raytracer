#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Vec3.h"
#include <vector>

class Triangle 
{
public: 
    Triangle();
    Triangle(int v0_, int v1_, int v2_, bool vertexNormals);
    int v0, v1, v2;
    bool vertexNormals;
    float texX = 0.0f;
    float texY = 0.0f;

    static float area(const Vec3& a, const Vec3& b, const Vec3& c);
    static float area(const Vec3& ab, const Vec3& ac);


    static bool isInside(const Triangle& t, const Vec3& p, const std::vector<Vec3>& vertices);
    static bool isInside(const Vec3& a, const Vec3& b, const Vec3& c, const Vec3& p);
    
private: 
    bool isPointInside(const Vec3& v) const;
    
};

#endif
