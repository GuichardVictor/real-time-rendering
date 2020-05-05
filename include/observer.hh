#pragma once

#include "triangle.hh"
#include "Point2.hh"
#include "plane.hh"
#include "PlaneEquation.hh"

class Observer
{
public:
    Observer(const Point3& _center, const Point3& _objective, const Vector3& _up,
             int _width, int _height, float _zDist, float _openAngleX, float _openAngleY)
        :center_(_center), objective_(_objective), up_(_up), width_(_width),
         height_(_height), zDist_(_zDist), openAngleX_(_openAngleX),
         openAngleY_(_openAngleY)
    {}

    void initBuffer();

    void initFrustum();

    Point3 projectPoint(const Point3& p) const;

    void updateBuffer(Triangle &tr);
    
    Point3 getCoord(const Point2& p) const;

    bool isFrustumCulled(const Triangle& tr);

    void fillFlat(const Point2&a , const Point2& b,
                                   const Point2& c, PlaneEquation& eq,
                                   bool top, const Triangle& tr);

    Point2 computePointCoordinate(const Point3& p) const;

    Point3 center_;
    Point3 objective_;
    Vector3 up_;
    int width_;
    int height_;
    float zDist_;
    float openAngleX_;
    float openAngleY_;

    std::vector<Plane> frustum;
    std::vector<Point3> imagePlan;
    std::vector<float> depthBuffer;
    std::vector<Color> frameBuffer;
    std::vector<Triangle> triangleHit;

};
