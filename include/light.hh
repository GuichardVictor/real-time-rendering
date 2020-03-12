#pragma once

#include "image.hh"
#include "point3.hh"
#include "triangle.hh"
#include "Point2.hh"
#include "PlaneEquation.hh"


class Camera;

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


class DirectionalLight
{
    public:
    DirectionalLight(const Color& _color, const Point3& _center,
                     const Vector3& _direction, float _angle, int _width, int _height)
    : color(_color), center(_center), direction(_direction), angle(_angle), width(_width), height(_height)
    {}
    
    void initBuffer();

    void updateBuffer(Triangle& tr);

    Point3 projectPoint(const Point3& p) const;

    void fillFlat(const Point2&a , const Point2& b,
                                   const Point2& c, PlaneEquation& eq,
                                   bool top);

    Point3 getCoord(const Point2& p) const;

    Point2 computePointCoordinate(const Point3& p) const;


    
    Color color;
    Point3 center;
    Vector3 direction;
    float angle;
    int width;
    int height;

    //shadow map
    float zDist = 1;
    std::vector<Point3> imagePlan;
    std::vector<float> depthBuffer;
};