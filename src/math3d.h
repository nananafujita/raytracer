#ifndef MATH3D_H
#define MATH3D_H

#include <math.h>

static inline void subtract(const double a[3], const double b[3], double result[3]) {
    result[0] = a[0] - b[0];
    result[1] = a[1] - b[1];
    result[2] = a[2] - b[2];
}

static inline void add(const double a[3], const double b[3], double result[3]) {
    result[0] = a[0] + b[0];
    result[1] = a[1] + b[1];
    result[2] = a[2] + b[2];
}

static inline void normalize(double v[3]) {
    double magnitude = sqrt(pow(v[0], 2) + pow(v[1], 2) + pow(v[2], 2));
    v[0] /= magnitude;
    v[1] /= magnitude;
    v[2] /= magnitude;
}

static inline double dot(const double a[3], const double b[3]) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

static inline void cross(const double a[3], const double b[3], double result[3]) {
    result[0] = a[1] * b[2] - a[2] * b[1];
    result[1] = a[2] * b[0] - a[0] * b[2];
    result[2] = a[0] * b[1] - a[1] * b[0];
}

static inline void multiply(const double v[3], const double t, double result[3]) {
    result[0] = v[0] * t;
    result[1] = v[1] * t;
    result[2] = v[2] * t;
}

static inline void divide(const double v[3], const double t, double result[3]) {
    result[0] = v[0] / t;
    result[1] = v[1] / t;
    result[2] = v[2] / t;
}

static inline double max(const double a, const double b) {
    if (a > b)  return a;
    else        return b;
}

static inline double min(const double a, const double b) {
    if (a < b)  return a;
    else        return b;
}

static inline double length(const double a[3]) {
    return sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
}
static inline double squared_length(const double a[3]) {
    return a[0] * a[0] + a[1] * a[1] + a[2] * a[2];
}

static inline void interpolate(const double barycentric_coordinates[3], const double a[3], const double b[3], const double c[3], double result[3]) {
    double u[3];
    double v[3];
    double w[3];
    multiply(a, barycentric_coordinates[0], u);
    multiply(b, barycentric_coordinates[1], v);
    multiply(c, barycentric_coordinates[2], w);
    add(u, v, result);
    add(result, w, result);
}

#endif