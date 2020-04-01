#pragma once

#include "point3.hh"
#include "image.hh"
#include "vector3.hh"

class Triangle
{
public:
    Triangle()
    : a({0,0,0}), b({0,0,0}), c({0,0,0})
    {}
    Triangle(const Point3& _a, const Point3& _b, const Point3& _c,
             const Color& _color, float _specular = 1, float _diffuse = 1,
             float _ambient = 1, float _shininess = 10)
        : a(_a), b(_b), c(_c), color(_color),
          specular(_specular), diffuse(_diffuse),
          ambient(_ambient), shininess(_shininess)
    {}


    Point3 a;
    Point3 b;
    Point3 c;
    Color color;
    float specular;
    float diffuse;
    float ambient;
    float shininess;

    Vector3 normal = Vector3(0,0,0);
};
