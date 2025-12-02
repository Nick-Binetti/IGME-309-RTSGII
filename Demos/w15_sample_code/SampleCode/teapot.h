#ifndef TEAPOT_H
#define TEAPOT_H

#include "object.h"

class Teapot : public Object
{
private:
    float size; // Size of the teapot

public:
    // Constructor
    Teapot(float s = 1.0f) : Object(), size(s) {}

    // Set teapot size
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
        glutSolidTeapot(size);
    }

    // Optional: wireframe version
    void drawWireframe()
    {
        applyTransformations();
        glutWireTeapot(size);
        unapplyTransformations();
    }
};

#endif // TEAPOT_H
