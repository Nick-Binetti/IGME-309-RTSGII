#pragma once
#include <vector>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

using namespace std;

class PolyObject
{
private:
    std::vector<float> vertices;
    std::vector<float> color;
public:
    PolyObject();
    void addVertex(float x, float y);            // add vertex at the end of the vertex list
    void setColor(float cr, float cg, float cb); // set the color of this polygon
    unsigned int getVertNum() const;                   // return the number of vertices
    void draw() const;                                 // draw the polygon if it’s completed
    void draw(float mouseX, float mouseY); //draw the polygon in progress
};