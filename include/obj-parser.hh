#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "triangle.hh"
#include "vector3.hh"

class ObjParser
{
public:
    ObjParser() = default;
    bool parse(const std::string& filepath);

private:
    bool parse_object(std::ifstream& f);

public:
    std::vector<Point3> vertices;
    std::vector<Point3> texture_vertices;
    std::vector<Vector3> normal_vertices;

    std::vector<Triangle> triangles;
};
