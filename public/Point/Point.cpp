#include "Point.h"
Point::Point() {}

Point::Point(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

Point::~Point() {}

/*-------NormalPoint---------*/

NormalPoint::NormalPoint(/* args */){}

NormalPoint::NormalPoint(float x, float y, float z) : Point(x,y,z) {}

NormalPoint::~NormalPoint() {}

/*-------ColoredPoint---------*/

ColoredPoint::ColoredPoint() {}

ColoredPoint::ColoredPoint(float x, float y, float z): Point(x,y,z) {}

ColoredPoint::~ColoredPoint() {}