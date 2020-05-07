#pragma once

#include <fstream>
#include <string>

#include <GL/glew.h>
#include <GL/freeglut.h>

class Shader
{
public:
    Shader(const std::string& filepath, GLuint shader_type);

    GLuint id_get() const { return shader_id; }
    bool is_ok() const { return ready; }

protected:
    std::string shader_content;
    GLuint shader_id;
    bool ready;
};
