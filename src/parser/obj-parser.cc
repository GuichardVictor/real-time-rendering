#include "obj-parser.hh"

#include <sstream>
#include <algorithm>

bool is_comment(const std::string& line)
{
    size_t start = 0;
    while (line[start] == ' ')
        start ++;

    return line[start] == '#';

}

std::vector<std::string> split(const std::string& line)
{
    std::vector<std::string> cont;

    std::stringstream ss(line);
    std::string token;
    
    while (std::getline(ss, token, ' '))
    {
        cont.push_back(token);
    }

    cont.erase(std::remove_if(cont.begin(), cont.end(),
                              [](const std::string& x) {
                                    return x == "";
                              }), cont.end());

    return cont;
}


bool ObjParser::parse_object(std::ifstream& f)
{
    // Clearing previous settings
    vertices.clear();
    texture_vertices.clear();
    normal_vertices.clear();

    // Reading line
    std::string line;

    while (std::getline(f, line))
    {
        if (is_comment(line))
            continue;

        auto line_split = split(line);
        if (line_split.size() == 0)
            continue;

        if (line_split[0] == "o")
        {
            // tell that there is a new object to parse
            return true;
        }
        else if (line_split[0] == "v")
        {
            // Vertice
            vertices.push_back({std::stof(line_split[1]),
                                std::stof(line_split[2]),
                                std::stof(line_split[3])});

        } else if (line_split[0] == "vn")
        {
            // Normal Vector -- Not yet used
            normal_vertices.push_back({std::stof(line_split[1]),
                                       std::stof(line_split[2]),
                                       std::stof(line_split[3])});

        } else if (line_split[0] == "vt")
        {
            // Texture Vertice -- Not yet used
            texture_vertices.push_back({std::stof(line_split[1]),
                                        std::stof(line_split[2]),
                                        0.0}); // 0 as it is a 2d coordinate on texture

        } else if (line_split[0] == "f")
        {
            // Triangle
            // Convert char to int and the vertices are 1 indexed in .obj files
            triangles.push_back({
                vertices[(line_split[1][0] - '0') - 1],
                vertices[(line_split[2][0] - '0') - 1],
                vertices[(line_split[3][0] - '0') - 1],
                Color(0,1,0)
            });
        }
    }

    return false;
}


bool ObjParser::parse(const std::string& filepath)
{
    std::ifstream file{filepath};

    if (!file.is_open())
        return false;

    while (parse_object(file))
        continue;

    return true;
}
