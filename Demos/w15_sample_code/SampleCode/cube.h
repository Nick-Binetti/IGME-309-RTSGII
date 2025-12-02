#ifndef CUBE_H
#define CUBE_H

#include "object.h"

class Cube : public Object
{
private:
    float size; // Length of each edge

public:
    // Constructor
    Cube(float s = 1.0f) : Object(), size(s) {}

    // Set cube size
    void setSize(float s)
    {
        size = s;
    }

    float getSize() const
    {
        return size;
    }

    // Implement the draw method
    void draw() override
    {
        glutSolidCube(size);
    }

    // Optional: wireframe version
    void drawWireframe()
    {
        applyTransformations();
        glutWireCube(size);
        unapplyTransformations();
    }
};

#endif // CUBE_H
