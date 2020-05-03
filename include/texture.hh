#pragma once

#include <string>
#include <vector>

#include "program.hh"

class Texture
{
public:
    Texture() = default;
    Texture(const std::string& filepath, GLuint mode=GL_LINEAR);
    Texture(int width, int height);
    Texture(const std::vector<std::string>& faces);

    void add_to_program(Program& program, const std::string& name);

    void use();

    GLuint unit_get() const { return texture_unit; }
    GLuint id_get() const { return texture_id; }

    int width_get() const { return width_; }
    int height_get() const { return height_; }

    void update(int width, int height, const unsigned char* pixels);

private:

    int width_;
    int height_;

    GLuint texture_id;
    GLuint texture_unit;

    static unsigned char nb_textures;
};
