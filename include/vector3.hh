#pragma once

#include "point3.hh"

class Point3;

class Vector3 {
public:
    friend float dot(const Vector3& a, const Vector3& b);
    friend Vector3 crossProduct(const Vector3& a, const Vector3& b);

    Vector3() = default;
    Vector3(float x, float y, float z)
    : x_(x), y_(y), z_(z)
    {}

    Vector3(const Point3& a, const Point3& b)
    {
        x_ = b.x_ - a.x_;
        y_ = b.y_ - a.y_;
        z_ = b.z_ - a.z_;
    }

    friend std::ostream& operator<<(std::ostream &os, const Vector3 &v);
    Vector3 operator*(const float &l) const;
    Vector3 operator/(const float &l) const;
    Vector3 operator-(const Vector3 &v) const;
    Vector3 normalize() const;

    float norm();

public:
    float x_;
    float y_;
    float z_;
};