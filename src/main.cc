#include <vector>
#include <iostream>

#include "camera.hh"
#include "triangle.hh"

void renderScene(std::vector<Triangle> objects);

int main()
{
    std::vector<Triangle> objects;
    objects.push_back(Triangle({10,1,3} , {-5,1,2}, {15, -5, 2}, Color(0,1,0)));
    objects.push_back(Triangle({10,10,5} , {1,1,4}, {-1,1, 5}, Color(1,0,0)));
    renderScene(objects);
    return 0;
}

void renderScene(std::vector<Triangle> objects)
{
    Camera c = Camera(Point3(0,0,0), Point3(0,0,1), Vector3(0,1,0), 2.04, 2.04, 1);
    c.initPoints(WIDTH, HEIGHT);
    auto p = c.projectPoint({0,0,4});
    std::cout << p << std::endl;
    for(auto& obj : objects)
    {
        c.updateBuffer(obj);
    }
    Image img(WIDTH, HEIGHT);
    img.pixels = c.frameBuffer;
    img.save("/home/ironfan/Ing2/ISIM/real-time-rendering/test.ppm");
}