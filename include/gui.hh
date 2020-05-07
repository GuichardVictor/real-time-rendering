#pragma once

#include <vector>

#include "texture.hh"

class GUI
{
public:
    GUI(int x, int y, const Texture& texture)
        : pos_x(x), pos_y(y), texture_{texture}
    { create_quad(); }

    GUI() = default;

    void draw()
    {
        glBindVertexArray(vao_id);
        texture_.use();
        //glBindTexture(GL_TEXTURE_2D, texture_.id_get());
        glDrawArrays(GL_TRIANGLES, 0, 6);//quad_vertices.size());
    }

    void attach_texture(Texture& texture) { texture_ = texture; create_quad(); }

    void use_vertices(Program& p, const std::string& name);
    void use_texture_coords(Program& p, const std::string& name);

private:
    void create_quad();

private:
    int pos_x;
    int pos_y;
    Texture texture_;

    std::vector<float> quad_vertices;
    GLuint vao_id;
    GLuint vbo_id;
};
