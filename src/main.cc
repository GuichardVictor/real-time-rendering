#include <vector>
#include <iostream>

#include "camera.hh"
#include "triangle.hh"
#include "obj-parser.hh"


#include <chrono> 

#include <SDL2/SDL.h>

#define SPEED 0.1
void renderScene(std::vector<Triangle>& objects, bool debug = false);
void renderSpecificScene(std::vector<Triangle>& objects, bool debug = false);


struct SDL_Wrapper
{
    SDL_Wrapper(int width_, int height_)
    {
        width = width_;
        height = height_;

        // SDL_INIT will generate may invalid write / read
        // It will stop as soon as the first frame is fully rendered
        SDL_Init(SDL_INIT_VIDEO);

        window = SDL_CreateWindow("SDL2 Pixel Drawing",
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  width, height, 0);

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_SOFTWARE);

        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);
        format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);

    }

    ~SDL_Wrapper()
    {
        SDL_FreeFormat(format);
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void render()
    {
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    void update_pixels(const std::vector<Color>& frameBuffer)
    {
        SDL_LockTexture(texture, NULL, (void**)&pixels, &pitch);

        for (int i = 0; i < WIDTH * HEIGHT; i++)
        {
            auto col = frameBuffer[i];
            auto red = (col.red < 1) ? col.red * 255 : 255;
            auto green = (col.green < 1) ? col.green * 255 : 255;
            auto blue = (col.blue < 1) ? col.blue * 255 : 255;
            pixels[i] = SDL_MapRGBA(format, red, green, blue, 255);
        }

        SDL_UnlockTexture(texture);
    }

    SDL_Event event;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    SDL_PixelFormat* format;


    uint32_t *pixels;
    int pitch;

    int width;
    int height;
};



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
    //renderScene(objs.triangles, false);
    renderSpecificScene(objs.triangles, true);
    /*
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
    c.computeAllColors();
    //c.addShadow();
    img.pixels = c.frameBuffer;
    img.save("test.ppm");
}

void renderScene(std::vector<Triangle>& objects, bool debug)
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

    SDL_Wrapper sdl_main{WIDTH, HEIGHT};

    auto start = std::chrono::high_resolution_clock::now();
    int frameCounter = 0;
    
    bool quit = false;
    while (!quit)
    {

        SDL_WaitEvent(&sdl_main.event);
        Point3 center = c.center_;
        Point3 objective = c.objective_;

        int relX;
        int relY;

        // FIX SDL INPUT TO REDUCE LAGS!
        switch (sdl_main.event.type)
        {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                /* Check the SDLKey values and move change the coords */
                switch( sdl_main.event.key.keysym.sym ){
                    case SDLK_q:
                        center.x_ += SPEED;
                        objective.x_ += SPEED;
                        c = Camera(center, objective, globalUp, globalRight, 2.04, 2.04);
                        c.initBuffer();
                        c.lights = lights;
                        break;
                    case SDLK_d:
                        center.x_ -= SPEED;
                        objective.x_ -= SPEED;
                        c = Camera(center, objective, globalUp, globalRight, 2.04, 2.04);
                        c.initBuffer();
                        c.lights = lights;
                        break;
                    case SDLK_z:
                        center.z_ += SPEED;
                        objective.z_ += SPEED;
                        c = Camera(center, objective, globalUp, globalRight, 2.04, 2.04);
                        c.initBuffer();
                        c.lights = lights;
                        break;
                    case SDLK_s:
                        center.z_ -= SPEED;
                        objective.z_ -= SPEED;
                        c = Camera(center, objective, globalUp, globalRight, 2.04, 2.04);
                        c.initBuffer();
                        c.lights = lights;
                        break;
                    case SDLK_SPACE:
                        center.y_ += SPEED;
                        objective.y_ += SPEED;
                        c = Camera(center, objective, globalUp,globalRight, 2.04, 2.04);
                        c.initBuffer();
                        c.lights = lights;
                        break;
                    case SDLK_LCTRL:
                        center.y_ -= SPEED;
                        objective.y_ -= SPEED;
                        c = Camera(center, objective, globalUp, globalRight, 2.04, 2.04);
                        c.initBuffer();
                        c.lights = lights;
                        break;
                    default:
                        break;
                
                }
                std::cout << c.center_<<std::endl;
                break;
            case SDL_MOUSEMOTION:
                SDL_GetRelativeMouseState(&relX, &relY);
                if (relX != 0 || relY != 0)
                {
                    // Handle camera rotation
                    objective.x_ += (float)relX * 0.001;
                    //objective.y_ += (float)relY * 0.00001;
                    // Cannot change up with mouse as it does not work :/

                    c = Camera(center, objective, globalUp, globalRight, 2.04, 2.04);
                    c.initBuffer();
                    c.lights = lights;
                }
                break;
        }
        for(auto& obj : objects)
        {
            c.updateBuffer(obj);
        }
        c.computeAllColors();

        sdl_main.update_pixels(c.frameBuffer);
        sdl_main.render();
        
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);

        if (duration.count() == 1)
        {
            std::cout << frameCounter << '\n';
            start = std::chrono::high_resolution_clock::now();
            frameCounter = 0;
        }
        frameCounter += 1;

    }
 



/*
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
    for(auto &light : c.lights)
    {
        light.initBuffer();
        for(auto& obj: objects)
        {
            light.updateBuffer(obj);
            if(debug)
            {
                std::string filename = "imageslight/test";
                img.pixels = light.frameBuffer;
                img.save(filename.append(std::to_string(i)).append(".ppm"));
                i++;
            }
        }
    }
    c.addShadow();
    img.pixels = c.frameBuffer;
    img.save("test.ppm");*/
}