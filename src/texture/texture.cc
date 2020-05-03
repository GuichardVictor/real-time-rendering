#include "texture.hh"

#include <iostream>

unsigned char Texture::nb_textures = 0;

Texture::Texture(const std::string& filepath, GLuint mode)
{
    glGenTextures(1, &texture_id);
    glActiveTexture(GL_TEXTURE0 + nb_textures);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    auto c = filepath.c_str();
    std::cout << c[0];
    mode = mode;
    /*
    auto img = tifo::load_image(filepath.c_str());
    if (img)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                     img->sx, img->sy, 0, GL_RGB, GL_UNSIGNED_BYTE, img->pixels);

    texture_unit = nb_textures;
    nb_textures += 1;
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);*/
}

Texture::Texture(const std::vector<std::string>& faces)
{
    auto size = faces.size();
    size = size;
    /*
    glGenTextures(1, &texture_id);
    glActiveTexture(GL_TEXTURE0 + nb_textures);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

    for (unsigned int i = 0; i < faces.size(); i++)
    {
        auto img = tifo::load_image(faces[i].c_str());
        if (img)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0, GL_RGB, img->sx, img->sy, 0, GL_RGB, GL_UNSIGNED_BYTE, img->pixels);
            width_ = img->sx;
            height_ = img->sy;
            delete img;
        }
        else
        {
            std::cerr << "Image: error: " << faces[i] << " can't be read." << std::endl;
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    texture_unit = nb_textures;
    nb_textures += 1;*/

}

void Texture::update(int width, int height, const unsigned char* pixels)
{
    if (texture_id == 0)
    {
        glGenTextures(1, &texture_id);
        glActiveTexture(GL_TEXTURE0 + nb_textures);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                     width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        texture_unit = nb_textures;
        nb_textures += 1;
        return;
    }

    glActiveTexture(GL_TEXTURE0 + texture_unit);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
}


void Texture::add_to_program(Program& program, const std::string& name)
{
    program.create_uniform(name);

    auto loc = program.get_uniform(name);
    glUniform1i(loc, texture_unit);
}

Texture::Texture(int width, int height)
    : width_{width}, height_{height},
      texture_id{}, texture_unit(nb_textures)
{
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::use()
{
    glActiveTexture(GL_TEXTURE0 + texture_unit);
    glBindTexture(GL_TEXTURE_2D, texture_id);
}
