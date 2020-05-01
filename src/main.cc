#include <vector>
#include <iostream>

#include "camera.hh"
#include "triangle.hh"
#include "obj-parser.hh"


#include <chrono> 

#include <SDL2/SDL.h>

#define SPEED 0.1
void renderSpecificScene(std::vector<Triangle>& objects, bool debug = false);

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
    renderSpecificScene(objs.triangles, true);
}

void renderSpecificScene(std::vector<Triangle>& objects, bool debug)
{
    
    if(debug)
        debug = true;
    Vector3 globalUp = Vector3(0,1,0);
    Vector3 globalRight = Vector3(1,0,0);

    Camera c = Camera(Point3(1.5,1.5,2.5), Point3(1.4,1.5,2.4), globalUp, globalRight, 2.04, 2.04);
    std::vector<DirectionalLight> lights;
    lights.push_back(DirectionalLight(Color(1,1,1), {2.5,1.5, 2.5}, {2.4,1.5,2.4},
                                      Vector3(0,1,0), 2.04, WIDTH, HEIGHT));
    c.initBuffer();
    c.lights = lights;
    Image img(WIDTH, HEIGHT);
    for(auto& obj : objects)
    {
        c.updateBuffer(obj);
    }
    c.lights[0].initBuffer();
    for(auto& obj : objects)
    {
        c.lights[0].updateBuffer(obj);
    }
    c.computeAllColors();
    c.addShadow();
    img.pixels = c.frameBuffer;
    img.save("test.ppm");
}