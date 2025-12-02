#ifndef OBJECT_H
#define OBJECT_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

class Object
{
protected:
    // Position
    float x, y, z;

    // Rotation (in degrees)
    float rotX, rotY, rotZ;

    // Scale
    float scaleX, scaleY, scaleZ;

    // Color (RGBA)
    float r, g, b, a;

public:
    // Constructor
    Object() : x(0.0f), y(0.0f), z(0.0f),
               rotX(0.0f), rotY(0.0f), rotZ(0.0f),
               scaleX(1.0f), scaleY(1.0f), scaleZ(1.0f),
               r(1.0f), g(1.0f), b(1.0f), a(1.0f) {}

    virtual ~Object() {}

    // Transformation methods
    void setPosition(float px, float py, float pz)
    {
        x = px;
        y = py;
        z = pz;
    }

    void translate(float dx, float dy, float dz)
    {
        x += dx;
        y += dy;
        z += dz;
    }

    void setRotation(float rx, float ry, float rz)
    {
        rotX = rx;
        rotY = ry;
        rotZ = rz;
    }

    void rotate(float dx, float dy, float dz)
    {
        rotX += dx;
        rotY += dy;
        rotZ += dz;
    }

    void setScale(float sx, float sy, float sz)
    {
        scaleX = sx;
        scaleY = sy;
        scaleZ = sz;
    }

    void setUniformScale(float s)
    {
        scaleX = scaleY = scaleZ = s;
    }

    void setColor(float red, float green, float blue, float alpha = 1.0f)
    {
        r = red;
        g = green;
        b = blue;
        a = alpha;
    }

    // Getters
    void getPosition(float &px, float &py, float &pz) const
    {
        px = x;
        py = y;
        pz = z;
    }

    void getRotation(float &rx, float &ry, float &rz) const
    {
        rx = rotX;
        ry = rotY;
        rz = rotZ;
    }

    void getScale(float &sx, float &sy, float &sz) const
    {
        sx = scaleX;
        sy = scaleY;
        sz = scaleZ;
    }

    // Apply transformations (to be called before drawing)
    void applyTransformations()
    {
        glPushMatrix();

        // Apply translation
        glTranslatef(x, y, z);

        // Apply rotation (order: Z, Y, X)
        glRotatef(rotZ, 0.0f, 0.0f, 1.0f);
        glRotatef(rotY, 0.0f, 1.0f, 0.0f);
        glRotatef(rotX, 1.0f, 0.0f, 0.0f);

        // Apply scale
        glScalef(scaleX, scaleY, scaleZ);

        // Apply color
        glColor4f(r, g, b, a);
    }

    void unapplyTransformations()
    {
        glPopMatrix();
    }

    // Pure virtual draw method - must be implemented by subclasses
    virtual void draw() = 0;

    // Convenience method that applies transformations, draws, and unapplies
    void render()
    {
        applyTransformations();
        draw();
        unapplyTransformations();
    }
};

#endif // OBJECT_H
