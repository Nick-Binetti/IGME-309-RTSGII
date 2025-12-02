#ifndef DUCK_H
#define DUCK_H

#include "object.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class Duck : public Object
{
private:
    float size;

public:
    // Constructor
    Duck(float s = 1.0f) : Object(), size(s) {}

    // Set duck size
    void setSize(float s)
    {
        size = s;
    }

    float getSize() const // Const member function.
    {
        return size;
    }

    // Implement the draw method using GLUT primitives
    // This function overrides a virtual function from the base class.
    void draw() override
    {
        // Body - sphere (slightly flattened with scale)
        glPushMatrix();
        glTranslatef(0.0f, -0.3f * size, 0.0f);
        glScalef(1.3f, 0.9f, 1.0f); // Make it wider and flatter
        glutSolidSphere(size, 20, 20);
        glPopMatrix();

        // Neck - cone (inverted, connecting body to head)
        glPushMatrix();
        glTranslatef(0.0f, 0.5f * size, 0.0f);
        glRotatef(-90, 1.0f, 0.0f, 0.0f); // Point upward
        glutSolidCone(0.3f * size, 0.7f * size, 12, 12);
        glPopMatrix();

        // Head - sphere
        glPushMatrix();
        glTranslatef(0.0f, 1.1f * size, 0.0f);
        glutSolidSphere(0.5f * size, 15, 15);
        glPopMatrix();

        // Beak - cone (pointing forward)
        glPushMatrix();
        glTranslatef(0.45f * size, 1.0f * size, 0.0f);
        glRotatef(90, 0.0f, 1.0f, 0.0f); // Point forward along X-axis
        glScalef(1.0f, 0.6f, 0.6f);      // Flatten it a bit
        glutSolidCone(0.2f * size, 0.5f * size, 10, 10);
        glPopMatrix();

        // Eyes - small spheres
        glPushMatrix();
        glTranslatef(0.25f * size, 1.25f * size, 0.3f * size);
        glutSolidSphere(0.08f * size, 8, 8);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.25f * size, 1.25f * size, -0.3f * size);
        glutSolidSphere(0.08f * size, 8, 8);
        glPopMatrix();

        // Tail - cone (pointing backward and upward)
        glPushMatrix();
        glTranslatef(-1.2f * size, -0.1f * size, 0.0f);
        glRotatef(-70, 0.0f, 1.0f, 0.0f); // Angle it backward
        glRotatef(20, 0.0f, 0.0f, 1.0f);  // Tilt up slightly
        glutSolidCone(0.25f * size, 0.6f * size, 10, 10);
        glPopMatrix();

        // Left Wing - flattened sphere
        glPushMatrix();
        glTranslatef(-0.5f * size, -0.1f * size, 0.85f * size);
        glRotatef(-15, 1.0f, 0.0f, 0.0f); // Angle down
        glScalef(0.8f, 0.4f, 0.2f);       // Flatten into wing shape
        glutSolidSphere(0.6f * size, 12, 12);
        glPopMatrix();

        // Right Wing - flattened sphere
        glPushMatrix();
        glTranslatef(-0.5f * size, -0.1f * size, -0.85f * size);
        glRotatef(15, 1.0f, 0.0f, 0.0f); // Angle down
        glScalef(0.8f, 0.4f, 0.2f);      // Flatten into wing shape
        glutSolidSphere(0.6f * size, 12, 12);
        glPopMatrix();

        // Left Foot - cube (flattened)
        glPushMatrix();
        glTranslatef(0.0f * size, -1.2f * size, 0.5f * size);
        glScalef(0.4f, 0.1f, 0.8f); // Flat and wide
        glutSolidCube(size * 0.5f);
        glPopMatrix();

        // Right Foot - cube (flattened)
        glPushMatrix();
        glTranslatef(0.0f * size, -1.2f * size, -0.5f * size);
        glScalef(0.4f, 0.1f, 0.8f); // Flat and wide
        glutSolidCube(size * 0.5f);
        glPopMatrix();
    }

    // Optional: wireframe version
    void drawWireframe()
    {
        applyTransformations();
        glutWireSphere(size, 12, 12);
        unapplyTransformations();
    }
};

#endif // DUCK_H
