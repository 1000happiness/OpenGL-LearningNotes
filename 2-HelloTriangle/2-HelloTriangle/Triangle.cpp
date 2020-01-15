#include "Triangle.h"

Triangle::Triangle() {}

Triangle::Triangle(Point a, Point b, Point c) {
    this->a = a;
    this->b = b;
    this->c = c;
}

Triangle::~Triangle() {}

void Triangle::formatVertices(float *vertices) {
    vertices[0] = this->a.x;
    vertices[1] = this->a.y;
    vertices[2] = this->a.z;
    vertices[3] = this->b.x;
    vertices[4] = this->b.y;
    vertices[5] = this->b.z;
    vertices[6] = this->c.x;
    vertices[7] = this->c.y;
    vertices[8] = this->c.z;
}
