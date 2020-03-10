#include <vector>
#include <iostream>

#include "camera.hh"
#include "triangle.hh"
#include "obj-parser.hh"

void renderScene(std::vector<Triangle> objects);

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        std::cerr << "usage : "<< argv[0] << " filepath\n";
        return 1;
    }
    ObjParser objs = ObjParser();
    auto ret = objs.parse(argv[1]);
    if (!ret)
    {
        std::cout << "something went wrong\n";
    }
    renderScene(objs.triangles);
    return 0;
}

void renderScene(std::vector<Triangle> objects)
{
    Camera c = Camera(Point3(0,0,15), Point3(0,0,14), Vector3(0,1,0), 2.04, 2.04, 1);
    std::vector<Light> lights;
    lights.push_back(Light(Color(1,1,1), {0,15,0}));
    c.initPoints(WIDTH, HEIGHT);
    c.lights = lights;
    for(auto& obj : objects)
    {
        c.updateBuffer(obj);
    }
    Image img(WIDTH, HEIGHT);
    img.pixels = c.frameBuffer;
    img.save("/home/ironfan/Ing2/ISIM/real-time-rendering/test.ppm");
}