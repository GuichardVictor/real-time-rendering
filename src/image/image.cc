#include "image.hh"

#include <fstream>

#define PIXEL_MAX 255

bool Image::save(const std::string& filename)
{
    std::ofstream file{filename};

    if (!file.is_open())
        return false;

    file << "P3\n";
    file << width << ' ' << height << '\n';
    file << PIXEL_MAX << '\n';

    for (size_t h = 0; h < height; h++)
    {
        for (size_t w = 0; w < width; w++)
        {
            auto pixel = pixels[h * height + w];

            file << std::to_string(pixel.red) << ' '
                 << std::to_string(pixel.green) << ' '
                 << std::to_string(pixel.blue) << ' ';

            file << " ";
        }
        file << '\n';
    }

    return true;
}

bool Image::set_pixel(size_t w, size_t h, const Color& c)
{
    auto pos = h * height + w;
    if (pos >= width * height)
        return false;

    pixels[pos] = c;
    return true;
}
