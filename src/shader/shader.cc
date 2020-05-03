#include "shader.hh"

#include <iostream>
#include <sstream>

Shader::Shader(const std::string& filepath, GLuint shader_type)
{
    std::ifstream reader;
    try
    {
        reader.open(filepath);
        std::stringstream ss;
        ss << reader.rdbuf();
        shader_content = ss.str();
        reader.close();
    } catch(std::ifstream::failure& e)
    {
        std::cerr << "Error shader: reading " << filepath << std::endl;
    }

    const GLchar* code_content = shader_content.c_str();

    shader_id = glCreateShader(shader_type);
    glShaderSource(shader_id, 1, &code_content, 0);
    glCompileShader(shader_id);


    // Error Handling
    GLint log_size;
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_size);

    ready = log_size == 0;
    if (!ready)
    {
        auto shader_log = new char[log_size + 1];
        glGetShaderInfoLog(shader_id, log_size, &log_size, shader_log);
        std::cerr << "Error shader:" << filepath << ": " << shader_log << std::endl;
        delete[] shader_log;
    }
}
