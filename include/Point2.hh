#pragma once

#include <iostream>

struct Point2
{
    Point2(int x_, int y_)
    :x(x_), y(y_)
    {}

    void print()
    {
        std::cout << "Point2:(x=" << x << ",y=" << y << ")\n"; 
    }
    int x;
    int y;
};

