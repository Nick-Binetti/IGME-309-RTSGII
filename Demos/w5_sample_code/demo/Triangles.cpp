#include "Triangles.h"

// Define the global pointer
App *app = nullptr;

// -------- Triangle implementation --------
Triangle::Triangle(float cx, float cy, float cr, float cg, float cb)
    : x(cx), y(cy), r(cr), g(cg), b(cb) {}

void Triangle::draw() const
{
    glColor3f(r, g, b);

    glBegin(GL_TRIANGLES);
    glVertex2f(x - 0.1f, y - 0.1f);
    glVertex2f(x + 0.1f, y - 0.1f);
    glVertex2f(x, y + 0.1f);
    glEnd();
}

// -------- App implementation --------
void App::render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    for (auto &t : triangles)
    {
        t.draw();
    }

    glutSwapBuffers();
}
