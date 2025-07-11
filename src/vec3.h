#ifndef VEC3_h
#define VEC3_H

class Vec3 {
public:
    double x, y, z;
    Vec3() : x(0), y(0), z(0) {};
    Vec3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {};
    //Vec3(Vec3& v) : x(v.x), y(v.y), z(v.z) {};

    Vec3& operator+=(const Vec3& v) {
        x += v.x; y+= v.y; z += v.z;
        return *this;
    };
    Vec3& operator-=(const Vec3& v) {
        x -= v.x; y-= v.y; z -= v.z;
        return *this;
    };
    Vec3& operator*=(const double t) {
        x *= t; y *= t; z *= t;
        return *this;
    };
    Vec3& operator/=(const double t){
        x /= t; y /= t; z /= t;
        return *this;
    };
    Vec3 operator-() const { return Vec3(-x, -y, -z); }
};

inline Vec3 operator-(const Vec3& v, const Vec3& u) {
    return Vec3(v.x - u.x, v.y - u.y, v.z - u.z);
}

inline Vec3 operator+(const Vec3& v, const Vec3& u) {
    return Vec3(v.x + u.x, v.y + u.y, v.z + u.z);
}

inline Vec3 operator*(const Vec3& v, const double t) {
    return Vec3(v.x * t, v.y * t, v.z * t);
}

// component-wise multiplication
inline Vec3 operator*(const Vec3& v, const Vec3& u) {
    return Vec3(v.x * u.x, v.y * u.y, v.z * u.z);
}

inline Vec3 operator/(const Vec3& v, const double t) {
    return Vec3(v.x / t, v.y / t, v.z / t);
}

inline double squared_length(const Vec3& v) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

inline double length(const Vec3& v) {
    return sqrt(squared_length(v));
}

inline void normalize(Vec3& v) {
    v /= length(v);
}

inline double dot(const Vec3& v, const Vec3& u) {
    return v.x* u.x + v.y * u.y + v.z * u.z;
}

inline Vec3 cross(const Vec3& v, const Vec3& u) {
    return Vec3(v.y * u.z - v.z * u.y,
                v.z * u.x - v.x * u.z,
                v.x * u.y - v.y * u.x);
}

inline Vec3 interpolate(const Vec3& bary, const Vec3& v, const Vec3& u, const Vec3& w) {
    Vec3 a = v * bary.x;
    Vec3 b = u * bary.y;
    Vec3 c = w * bary.z;
    return a + b + c;
}


#endif