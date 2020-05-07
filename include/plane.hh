#pragma once

#include "vector3.hh"
#include "point3.hh"

struct Plane
{
    Plane(const Point3& c, const Vector3& n)
    : center(c), normal(n)
    {}

    Point3 center;
    Vector3 normal;
};