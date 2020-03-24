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
                     const Point3& _objective, const Vector3& _up, float _angle, int _width, int _height)
    : color_(_color), center_(_center), objective_(_objective), up_(_up),
     angle_(_angle), width_(_width), height_(_height)
    {}
    
    void initBuffer();

    void updateBuffer(Triangle& tr);

    Point3 projectPoint(const Point3& p) const;

    void fillFlat(const Point2&a , const Point2& b,
                                   const Point2& c, PlaneEquation& eq,
                                   bool top, const Triangle& tr);

    Point3 getCoord(const Point2& p) const;

    Point2 computePointCoordinate(const Point3& p) const;


    Color computeColor(int x, int y, float z, const Triangle& tr);
    
    Color color_;
    Point3 center_;
    Point3 objective_;
    Vector3 up_;
    float angle_;
    int width_;
    int height_;

    //shadow map
    float zDist_ = 1;
    Vector3 right_;
    std::vector<Point3> imagePlan;
    std::vector<float> depthBuffer;
    std::vector<Color> frameBuffer;
};