#pragma once

#include <vector>
#include "vector3.hh"
#include "light.hh"
#include "triangle.hh"
#include "Point2.hh"
#include "PlaneEquation.hh"


#define HEIGHT 1080
#define WIDTH 1080


class Camera {

public:
    Camera(const Point3& center, const Point3& objective,
     const Vector3& globalUp, const Vector3& globalRight, float x, float y)
    : center_(center), objective_(objective), globalUp_(globalUp),
     globalRight_(globalRight), openFieldX_(x), openFieldY_(y)
    {
        init();
    }

    void init();

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

    void addShadow();

    Point3 center_;
    Point3 objective_;
    Vector3 globalUp_;
    Vector3 globalRight_;
    float openFieldX_;
    float openFieldY_;
    float zDist_;
    std::vector<Point3> imagePlan;
    std::vector<Color> frameBuffer;
    std::vector<float> depthBuffer;
    std::vector<DirectionalLight> lights;
    std::vector<Point3> targets;
    Vector3 up_;
};