#pragma once

#include "image.hh"
#include "point3.hh"
#include "triangle.hh"
#include "Point2.hh"
#include "PlaneEquation.hh"
#include "observer.hh"

class Light
{
    public:
    Light(const Color& _color, const Point3& _center)
    : color(_color), center(_center)
    {}

    Color color;
    Point3 center;
    std::vector<float> zbuffer;
};


class DirectionalLight : public Observer
{
    public:
    DirectionalLight(const Color& _color, const Point3& _center,
                     const Point3& _objective, const Vector3& _up,
                     float _angle, int _width, int _height, float zDist)
    : Observer(_center, _objective, _up, _width, _height, zDist, _angle, _angle)
    {
        color_ = _color;
    }

    Color computeColor(int x, int y, float z, const Triangle& tr);

    void computeAllColors();
    
    Color color_;
};