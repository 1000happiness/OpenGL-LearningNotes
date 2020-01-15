#pragma once
#include "../../public/Point/Point.h"

class Triangle
{
public:
    Point a,b,c;

    Triangle();
    Triangle(Point a, Point b, Point c);
    ~Triangle();

    void formatVertices(float *);
};

