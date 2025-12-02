#ifndef CONE_H
#define CONE_H

#include "object.h"

class Cone : public Object
{
private:
    float base;   // Radius of the base
    float height; // Height of the cone
    int slices;   // Number of subdivisions around the z-axis
    int stacks;   // Number of subdivisions along the z-axis

public:
    // Constructor
    Cone(float b = 1.0f, float h = 1.0f, int sl = 20, int st = 20)
        : Object(), base(b), height(h), slices(sl), stacks(st) {}

    // Set cone parameters
    void setBase(float b)
    {
        base = b;
    }

    void setHeight(float h)
    {
        height = h;
    }

    void setDetail(int sl, int st)
    {
        slices = sl;
        stacks = st;
    }

    float getBase() const
    {
        return base;
    }

    float getHeight() const
    {
        return height;
    }

    // Implement the draw method
    void draw() override
    {
        glutSolidCone(base, height, slices, stacks);
    }

    // Optional: wireframe version
    void drawWireframe()
    {
        applyTransformations();
        glutWireCone(base, height, slices, stacks);
        unapplyTransformations();
    }
};

#endif // CONE_H
