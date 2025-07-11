#ifndef HITTABLE_H
#define HITTABLE_H

#include "vec3.h"

struct HitInfo {
    bool hit = false;
    bool is_sphere;
    double t;
    Vec3 pos;
    Vec3 normal;        // normal (only for spheres; triangles interpolate useing barycentric)
    Vec3 bary;          // barycentric coordinates (only for triangles)
    const Hittable* object = nullptr;
};

struct Hittable {
public: 
    virtual ~Hittable() {};
    virtual bool hit(const Vec3& orig, const Vec3& dir, HitInfo& hit) = 0;
};

struct Sphere : public Hittable {
    Vec3 center;
    Vec3 diffuse;
    Vec3 specular;
    double shine;
    double radius;

    virtual bool hit(const Vec3& orig, const Vec3& dir, HitInfo& hit) override;
};

struct TriangleVertex{
    Vec3 pos;
    Vec3 normal;
    Vec3 diffuse;
    Vec3 specular;
    double shine;
};

struct Triangle : Hittable{
    TriangleVertex v[3];

    virtual bool hit(const Vec3& orig, const Vec3& dir, HitInfo& hit) override;
    double area(const Vec3& a, const Vec3& b, const Vec3& c, char ignore_axis);
};

struct Light {
    Vec3 pos;
    Vec3 color;
};



#endif