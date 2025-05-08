#ifndef VEC3_H
#define VEC3_H

#include <math.h>

typedef struct {
    double x, y, z;
} Vec3;

static inline Vec3 subtract(Vec3 a, Vec3 b) {
    return (Vec3){a.x - b.x, a.y - b.y, a.z - b.z};
}

static inline Vec3 add(Vec3 a, Vec3 b) {
    return (Vec3){a.x + b.x, a.y + b.y, a.z + b.z};
}

static inline void normalize(Vec3* v) {
    double magnitude = sqrt(pow(v->x, 2) + pow(v->y, 2) + pow(v->z, 2));
    v->x /= magnitude;
    v->y /= magnitude;
    v->z /= magnitude;
}

static inline double dot(Vec3 a, Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline Vec3 multiply(double t, Vec3 v) {
    return (Vec3){t * v.x, t * v.y, t * v.z};
}

static inline Vec3 divide(Vec3 v, double t) {
    return multiply(1.0 / t, v);
}

static inline double max(double a, double b) {
    if (a > b)  return a;
    else        return b;
}

static inline double min(double a, double b) {
    if (a < b)  return a;
    else        return b;
}


#endif