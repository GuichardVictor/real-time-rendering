#pragma once

#include <vector>
#include "point3.hh"
#include "vector3.hh"
#include "image.hh"
#include "triangle.hh"

#define HEIGHT 720
#define WIDTH 1080

class Light
{
    public:
    Light(const Color& _color, const Point3& _center)
    : color(_color), center(_center)
    {}

    Color color;
    Point3 center;
};

struct Point2
{
    Point2(int x_, int y_)
    :x(x_), y(y_)
    {}

    void print()
    {
        std::cout << "Point2:(x=" << x << ",y=" << y << ")\n"; 
    }
    int x;
    int y;
};

struct PlaneEquation
{


    PlaneEquation(float a_, float b_, float c_, float d_)
    : a(a_), b(b_), c(c_), d(d_)
    {}
    float a;
    float b;
    float c;
    float d;
};


class Camera {

public:
    Camera(const Point3& center, const Point3& objective, const Vector3 &up, float x, float y, float z)
    : center_(center), objective_(objective), up_(up), openFieldX_(x), openFieldY_(y), zDist_(z)
    {}

    Point3 projectPoint(const Point3& p) const;

    void initPoints(int width, int height);

    const Point3 &getCenter() const;

    void updateBuffer(Triangle& t);

    void fillFlat(const Point2&a , const Point2& b,
                                   const Point2& c, PlaneEquation& eq,
                                   bool top, const Triangle& tr);

    Color computeColor(int x, int y, float z, const Triangle& tr);
    Point3 getCoord(const Point2& p) const;

    Point2 computePointCoordinate(const Point3& p) const;

    Point3 center_;
    Point3 objective_;
    Vector3 up_;
    float openFieldX_;
    float openFieldY_;
    float zDist_;
    std::vector<Point3> imagePlan;
    std::vector<Color> frameBuffer;
    std::vector<float> depthBuffer;
    std::vector<Light> lights;
};