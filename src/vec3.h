#pragma once


struct Vec3 {
    double x, y, z;

    Vec3 operator+(const Vec3& v) const {
        return Vec3{
            x + v.x,
            y + v.y,
            z + v.z
        };
    }

    Vec3 operator-(const Vec3& v) const {
        return Vec3{
            x - v.x,
            y - v.y,
            z - v.z
        };
    }

    Vec3 operator*(double s) const { //for multiplying by a separate variable, the copy s is already created so changing it wouldnt affect the caller
        return Vec3{
            x * s,
            y * s,
            z * s
        };
    }

    Vec3& operator+=(const Vec3& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vec3& operator -=(const Vec3& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    Vec3 operator/(const double s) const {
        return Vec3{
            x / s,
            y / s,
            z / s
        };
    }

    Vec3 operator-() const {
        return Vec3{
            -x,
            -y,
            -z
        };
    }
};


inline Vec3 operator*(double s, const Vec3& v) {
    return v * s;
}

inline double dot(const Vec3& v1, const Vec3& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline double norm2(const Vec3& v) {
    return dot(v, v);
}