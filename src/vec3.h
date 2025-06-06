#ifndef VEC3_h
#define VEC3_H

class Vec3 {
public:
    double v[3];
    Vec3() : v[0](0), v[1](0), v[2](0) {}
    Vec3(double x, double y, double z) : v[0](x), v[1](y), v[2](z) {}

    Vec3& operator+=(const Vec3& v);
    Vec3& operator-=(const Vec3& v);
    Vec3& operator*=(const double t);
    Vec3& operator/=(const double t);
};

inline Vec3 operator-(const Vec3& a, const Vec3& b) {
    return Vec3(a.v[0] - b.v[0], a.v[1] - b.v[1], a.v[2] - b.v[2]);
}

inline Vec3 operator+(const Vec3& a, const Vec3& b) {
    return Vec3(a.v[0] + b.v[0], a.v[1] + b.v[1], a.v[2] + b.v[2]);
}

inline Vec3 operator*(const Vec3& v, const double t) {
    return Vec3(v.v[0] * t, v.v[1] * t, v.v[2] * t);
}

inline Vec3 operator/(const Vec3& v, const double t) {
    return Vec3(v.v[0] / t, v.v[1] / t, v.v[2] / t);
}

inline double squared_length(const Vec3& v) {
    return a.v[0] * a.v[0] + a.v[1] * a.v[1] + a.v[2] * a.v[2];
}

inline double length(const Vec3& v) {
    return sqrt(squared_length(v));
}

inline void normalize(Vec3& v) {
    v /= length(v);
}

inline double dot(const Vec3& a, const Vec3& b) {
    return a.v[0]* b.v[0] + a.v[1] * b.v[1] + a.v[2] * b.v[2];
}

inline Vec3 cross(const Vec3& a, const Vec3& b) {
    return Vec3(a.v[1] * b.v[2] - a.v[2] * b.v[1],
                a.v[2] * b.v[0] - a.v[0] * b.v[2],
                a.v[0] * b.v[1] - a.v[1] * b.v[0]);
}

inline double max(const double a, const double b) {
    return (a > b) ? a : b;
}

inline double min(const double a, const double b) {
    return (a < b) ? a : b;
}

inline Vec3 interpolate(const Vec3& bary, const Vec3& a, const Vec3& b, const Vec3& c) {
    double u[3] = a * bary[0];
    double v[3] = b * bary[1];
    double w[3] = c * bary[2];
    return u + v + w;
}


#endif