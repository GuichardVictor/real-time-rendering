#include <vector>
#include <iostream>

#include "camera.hh"
#include "triangle.hh"
#include "obj-parser.hh"

void renderScene(std::vector<Triangle>& objects, bool debug = false);
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
    renderScene(objs.triangles);/*
    Vector3 globalUp = Vector3(0,1,0);
    Vector3 globalRight = Vector3(1,0,0);
    Camera c = Camera(Point3(0,0,0), Point3(0,0,1), globalUp, globalRight, 0.78, 0.78);
    c.initPoints(WIDTH, HEIGHT);
    auto testPos = c.projectPoint({0,1,2});
    auto testNeg = c.projectPoint({0,1,-1});

    auto pPos = c.computePointCoordinate(testPos);
    auto pNeg = c.computePointCoordinate(testNeg);
    pPos.print();
    pNeg.print();
    return 0;*/
}

void renderScene(std::vector<Triangle>& objects, bool debug)
{
    Vector3 globalUp = Vector3(0,1,0);
    Vector3 globalRight = Vector3(1,0,0);
    Camera c = Camera(Point3(2.5,2.5,2.5), Point3(2.5,2.4,2.5), globalUp, globalRight, 0.78, 0.78);
    std::vector<DirectionalLight> lights;
    lights.push_back(DirectionalLight(Color(1,1,1), {2.5,2.5, 2.5},
                                      Vector3(0,0,9), 0.5, WIDTH, HEIGHT));
    c.initPoints(WIDTH, HEIGHT);
    c.lights = lights;
    Image img(WIDTH, HEIGHT);
    int i = 0;
    for(auto& obj : objects)
    {
        c.updateBuffer(obj);
        if(debug)
        {
            std::string filename = "images/test";
            img.pixels = c.frameBuffer;
            img.save(filename.append(std::to_string(i)).append(".ppm"));
            i++;
        }
    }
    /*for(auto &light : c.lights)
    {
        light.initBuffer();
        for(auto& obj: objects)
        {
            light.updateBuffer(obj);
        }
    }
    c.addShadow();*/
    img.pixels = c.frameBuffer;
    img.save("test.ppm");
}