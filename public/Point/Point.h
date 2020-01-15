#pragma once
class Point
{
public:
    float x;
    float y;
    float z;

    Point();
    Point(float x, float y, float z);
    ~Point();
};

class NormalPoint: public Point
{
private:
    /* data */
public:
    NormalPoint();
    NormalPoint(float x, float y, float z);
    ~NormalPoint();
};


class ColoredPoint: public Point
{
private:
    /* data */
public:
    ColoredPoint(/* args */);
    ColoredPoint(float x, float y, float z);
    ~ColoredPoint();
};




