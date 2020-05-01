#pragma once
#include <ostream>
#include <cmath>


class Vector3;

class Point3
{

    friend std::ostream& operator<<(std::ostream &os, const Point3 &p);
public:
    Point3 (float x, float y, float z)
    : x_(x), y_(y), z_(z)
    {}

    Point3 operator- (const Point3& other) const ;

    Point3 operator+ (const Vector3& v) const;

    bool operator!= (const Point3& p)
    {
        return this->x_ != p.x_ || this->y_ != p.y_ || this->z_ != p.z_;
    }

    float getX() const;

    float getY() const;

    float getZ() const;

public:
    float x_;
    float y_;
    float z_;

};