#ifndef TRIANGLES_H
#define TRIANGLES_H

#ifdef __APPLE__
#include <GLUT/glut.h> // macOS
#else
#include <GL/freeglut.h> // Linux/Windows
#endif

#include <vector>

// ---------------- Triangle Class ----------------
class Triangle
{
public:
    float x, y;    // center position
    float r, g, b; // color

    Triangle(float cx, float cy, float cr, float cg, float cb);

    void draw() const;
};

// ---------------- App Class ----------------
class App
{
public:
    std::vector<Triangle> triangles;

    void render();
};

// global pointer to App (so GLUT callbacks can forward)
extern App *app;

#endif // TRIANGLES_H
