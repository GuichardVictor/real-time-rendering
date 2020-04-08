#include <vector>

#include <SDL2/SDL.h>

#include "image.hh"

#define SPEED 1


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

        for (int i = 0; i < width * height; i++)
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
