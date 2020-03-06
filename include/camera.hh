#pragma once

#include <vector>
#include "point3.hh"
#include "vector3.hh"

#define HEIGHT 100
#define WIDTH 100

class Camera {

public:
    Camera(const Point3& center, const Point3& objective, const Vector3 &up, float x, float y, float z)
    : center_(center), objective_(objective), up_(up), openFieldX_(x), openFieldY_(y), zDist_(z)
    {}

    Point3 projectPoint(const Point3& p) const;

    void initPoints(int width, int height);

    const Point3 &getCenter() const;

    Point3 center_;
    Point3 objective_;
    Vector3 up_;
    float openFieldX_;
    float openFieldY_;
    float zDist_;
    std::vector<Point3> imagePlan;
};