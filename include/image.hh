#pragma once

#include <string>
#include <vector>

struct Color
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

class Image
{
public:
    Image(size_t _width, size_t _height)
        : width(_width), height(_height), pixels{_width * _height}
    {}

    bool save(const std::string& filename);

    bool set_pixel(size_t w, size_t h, const Color& c);


public:
    size_t width;
    size_t height;

    std::vector<Color> pixels;
};
