#include "program.hh"

#include <iostream>

#define TEST_OPENGL_ERROR()                                                             \
  do {									\
    GLenum err = glGetError();					                        \
    if (err != GL_NO_ERROR) std::cerr << "OpenGL ERROR!" << __LINE__ << std::endl;      \
  } while(0)

Program::Program()
{
    program_id = glCreateProgram();
}

Program::Program(const std::vector<Shader>& shaders)
    : shaders_{shaders}, uniforms_{}, program_id{}
{
    program_id = glCreateProgram();

    for (const auto& shader : shaders)
    {
        glAttachShader(program_id, shader.id_get());
    }
}

void Program::attach_shader(const Shader& shader)
{
    glAttachShader(program_id, shader.id_get());
}

bool Program::link()
{
    glLinkProgram(program_id);
    
    GLint success;
    GLchar log[1024];

    glGetProgramiv(program_id, GL_LINK_STATUS, &success);
    if (success == 0)
    {
        glGetProgramInfoLog(program_id, 1024, NULL, log);
        std::cerr << "Program error: " << program_id << ": " << log << std::endl;
        return false;
    }
    return true;
}

void Program::use()
{
    glUseProgram(program_id);
}

bool Program::create_uniform(const std::string& var_name)
{
    if (uniforms_.find(var_name) != uniforms_.end())
        return false;

    auto loc = glGetUniformLocation(program_id, var_name.c_str());

    uniforms_.insert({var_name, loc});
    return true;
}

bool Program::create_attrib(const std::string& var_name)
{
    if (attribs_.find(var_name) != attribs_.end())
        return false;

    auto loc = glGetAttribLocation(program_id, var_name.c_str());
    
    attribs_.insert({var_name, loc});
    return true;
}
