#include <iostream>

#include "camera.hh"
#include "triangle.hh"

int main()
{
    Camera c = Camera(Point3(0,0,0), Point3(0,0,1), Vector3(0,1,0), 2.04, 2.04, 1);
    c.initPoints(WIDTH, HEIGHT);
    
    auto p = c.projectPoint({0,0,4});

    std::cout << p << std::endl;
    return 0;
}
