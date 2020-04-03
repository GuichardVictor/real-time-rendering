#pragma once

#include <vector>
#include "vector3.hh"
#include "light.hh"
#include "observer.hh"


#define HEIGHT 500
#define WIDTH 500


class Camera : public Observer {

public:
    Camera(const Point3& center, const Point3& objective,
     const Vector3& globalUp, const Vector3& globalRight, float x, float y)
    : Observer(center, objective, Vector3(0,0,0), WIDTH, HEIGHT, 1, x, y)
    {
        init(globalUp, globalRight);
    }

    void init(const Vector3& globalUp, const Vector3& globalRight);

    Color computeColor(int x, int y, float z, const Triangle& tr);

    void computeAllColors();

    void addShadow();
    
    std::vector<DirectionalLight> lights;
};