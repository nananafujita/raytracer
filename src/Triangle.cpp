#include "Triangle.h"

Triangle::Triangle()
: v0(-1), v1(-1), v2(-1), vertexNormals(false)
{
}

Triangle::Triangle(int v0_, int v1_, int v2_, bool vn)
: v0(v0_), v1(v1_), v2(v2_), vertexNormals(vn)
{
}

// Begin utility functions
float Triangle::area(const Point3& a, const Point3& b, const Point3& c)
{
    Vec3 ab = b - a;
    Vec3 ac = c - a;
    Vec3 perpendicular = Vec3::cross(ab, ac);
    return Vec3::magnitude(perpendicular) * 0.5;
}

float Triangle::area(const Vec3& ab, const Vec3& ac) {
    Vec3 perpendicular = Vec3::cross(ab, ac);
    return Vec3::magnitude(perpendicular) * 0.5;
}

bool Triangle::isInside(const Triangle& t, const Vec3& p, const std::vector<Vec3>& vertices)
{
    const Vec3& v0 = vertices[t.v0];
    const Vec3& v1 = vertices[t.v1];
    const Vec3& v2 = vertices[t.v2];
    
    float a = area(v0, v1, v2);
    float areaP12 = area(p, v1, v2);
    float areaP02 = area(p, v0, v2);
    float areaP01 = area(p, v0, v1);

    return std::abs(a - (areaP12 + areaP02 + areaP01)) < 1e-6;
}

bool Triangle::isInside(const Vec3& a, const Vec3& b, const Vec3& c, const Vec3& p)
{
    float areaTri = area(a, b, c);
    float areaP12 = area(p, b, c);
    float areaP02 = area(p, a, c);
    float areaP01 = area(p, a, b);
    return areaTri == areaP12 + areaP02 + areaP01;
}
// End utility functions