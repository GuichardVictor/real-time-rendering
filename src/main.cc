#include <vector>
#include <iostream>

#include "camera.hh"
#include "triangle.hh"
#include "obj-parser.hh"

void renderScene(std::vector<Triangle>& objects);

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        std::cerr << "usage : "<< argv[0] << "obj file\n";
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

void renderScene(std::vector<Triangle>& objects)
{
    Vector3 globalUp = Vector3(0,1,0);
    Camera c = Camera(Point3(0,0,10), Point3(0,0,9), globalUp, 2.04, 2.04);
    std::vector<DirectionalLight> lights;
    lights.push_back(DirectionalLight(Color(1,1,1), {0,0,10},
                                      Vector3(0,0,9), 0.5, WIDTH, HEIGHT));
    c.initPoints(WIDTH, HEIGHT);
    c.lights = lights;
    Image img(WIDTH, HEIGHT);
    for(auto& obj : objects)
    {
        c.updateBuffer(obj);
    }
    /*for(auto &light : lights)
    {
        light.initBuffer(c);
        for(auto& obj: objects)
        {
            light.updateBuffer(obj);
        }
    }*/
    img.pixels = c.frameBuffer;
    img.save("test.ppm");
}
