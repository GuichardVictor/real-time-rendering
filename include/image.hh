#pragma once

#include <string>
#include <vector>

struct Color
{
    Color() = default;
    Color(float r, float g, float b)
    :red(r), green(g), blue(b)
    {}

    Color operator*(float f) const
    {
        return Color(red * f, green * f, blue * f);
    }

    Color operator*(const Color c) const
    {
        return Color(red * c.red, green * c.green, blue * c.blue);
    }

    Color operator+(const Color c) const
    {
        return Color(red + c.red, green + c.green, blue + c.blue);
    }

    void clamp()
    {
        red = (red > 1) ? 1 : red;
        red = (red < 0) ? 0 : red;

        green = (green > 1) ? 1 : green;
        green = (green < 0) ? 0 : green;

        blue = (blue > 1) ? 1 : blue;
        blue = (blue < 0) ? 0 : blue;
    }
 
    float red = 0;
    float green = 0;
    float blue = 0;
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
