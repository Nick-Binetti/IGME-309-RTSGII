#ifndef SPHERE_H
#define SPHERE_H

#include "object.h"

class Sphere : public Object
{
private:
    float radius;
    int slices; // Number of subdivisions around the z-axis
    int stacks; // Number of subdivisions along the z-axis

public:
    // Constructor
    Sphere(float r = 1.0f, int sl = 20, int st = 20)
        : Object(), radius(r), slices(sl), stacks(st) {}

    // Set sphere parameters
    void setRadius(float r)
    {
        radius = r;
    }

    void setDetail(int sl, int st)
    {
        slices = sl;
        stacks = st;
    }

    float getRadius() const
    {
        return radius;
    }

    // Implement the draw method
    void draw() override
    {
        glutSolidSphere(radius, slices, stacks);
    }

    // Optional: wireframe version
    void drawWireframe()
    {
        applyTransformations();
        glutWireSphere(radius, slices, stacks);
        unapplyTransformations();
    }
};

#endif // SPHERE_H
