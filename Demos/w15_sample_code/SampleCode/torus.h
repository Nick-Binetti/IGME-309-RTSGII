#ifndef TORUS_H
#define TORUS_H

#include "object.h"

class Torus : public Object
{
private:
    float innerRadius; // Radius of the tube
    float outerRadius; // Radius from center to tube center
    int sides;         // Number of sides for each radial section
    int rings;         // Number of radial divisions

public:
    // Constructor
    Torus(float inner = 0.3f, float outer = 1.0f, int s = 20, int r = 20)
        : Object(), innerRadius(inner), outerRadius(outer), sides(s), rings(r) {}

    // Set torus parameters
    void setInnerRadius(float inner)
    {
        innerRadius = inner;
    }

    void setOuterRadius(float outer)
    {
        outerRadius = outer;
    }

    void setDetail(int s, int r)
    {
        sides = s;
        rings = r;
    }

    float getInnerRadius() const
    {
        return innerRadius;
    }

    float getOuterRadius() const
    {
        return outerRadius;
    }

    // Implement the draw method
    void draw() override
    {
        glutSolidTorus(innerRadius, outerRadius, sides, rings);
    }

    // Optional: wireframe version
    void drawWireframe()
    {
        applyTransformations();
        glutWireTorus(innerRadius, outerRadius, sides, rings);
        unapplyTransformations();
    }
};

#endif // TORUS_H
