#pragma once

#include "point3.hh"

class Triangle
{
public:
    Triangle(const Point3& _a, const Point3& _b, const Point3& _c)
        : a(_a), b(_b), c(_c)
    {}

    Point3 a;
    Point3 b;
    Point3 c;
};
