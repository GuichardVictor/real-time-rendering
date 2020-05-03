#pragma once

#include <vector>
#include <unordered_map>

#include "shader.hh"

class Program
{
public:
    Program(const std::vector<Shader>& shaders);
    Program();
    void attach_shader(const Shader& shader);

    bool link();
    void use();

    GLint get_uniform(const std::string& var_name) const { return uniforms_.at(var_name); }
    bool create_uniform(const std::string& var_name);

    GLint get_attrib(const std::string& var_name) const { return attribs_.at(var_name); }
    bool create_attrib(const std::string& var_name);

private:
    std::vector<Shader> shaders_;
    std::unordered_map<std::string, GLint> uniforms_;
    std::unordered_map<std::string, GLint> attribs_;
    GLuint program_id;
};
