#pragma once

#include <vector>
#include "vector3.hh"
#include "light.hh"
#include "observer.hh"


class Camera : public Observer {

public:
    Camera(const Point3& center, const Point3& objective,
     const Vector3& globalUp, const Vector3& globalRight, float x, float y, float zDist)
    : Observer(center, objective, Vector3(0,0,0), WIDTH, HEIGHT, zDist, x, y)
    {
        init(globalUp, globalRight);
    }

    void init(const Vector3& globalUp, const Vector3& globalRight);

    Color computeColor(int x, int y, float z, const Triangle& tr);

    void computeAllColors(bool with_antialiasing=false);

    void addShadow();
    
    std::vector<DirectionalLight> lights;


    static int HEIGHT;
    static int WIDTH;
};
