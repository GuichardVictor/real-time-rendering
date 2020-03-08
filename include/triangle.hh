#pragma once

#include "point3.hh"
#include "image.hh"

class Triangle
{
public:
    Triangle(const Point3& _a, const Point3& _b, const Point3& _c ,const Color& _color)
        : a(_a), b(_b), c(_c), color(_color)
    {}


    Point3 a;
    Point3 b;
    Point3 c;
    Color color;
};
