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

    Triangle(float cx, float cy, float cr, float cg, float cb)
        : x(cx), y(cy), r(cr), g(cg), b(cb) {}

    void draw() const
    {
        glColor3f(r, g, b);

        glBegin(GL_TRIANGLES);
        glVertex2f(x - 0.1f, y - 0.1f);
        glVertex2f(x + 0.1f, y - 0.1f);
        glVertex2f(x, y + 0.1f);
        glEnd();
    }
};

// ---------------- App Class ----------------
class App
{
public:
    std::vector<Triangle> triangles;

    void render()
    {
        glClear(GL_COLOR_BUFFER_BIT);

        for (auto &t : triangles)
        {
            t.draw();
        }

        glutSwapBuffers();
    }
};

// ---------------- Global Pointer ----------------
App *app;

// ---------------- Main ----------------
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutCreateWindow("Multiple Triangles Example");

    App myApp;
    app = &myApp;

    // Add a few triangles
    app->triangles.emplace_back(-0.5f, 0.0f, 1.0f, 0.0f, 0.0f); // red left
    app->triangles.emplace_back(0.5f, 0.0f, 0.0f, 1.0f, 0.0f);  // green right
    app->triangles.emplace_back(0.0f, 0.5f, 0.0f, 0.0f, 1.0f);  // blue top

    // Hook GLUT to app->render
    glutDisplayFunc(+[]
                    { app->render(); });

    glutMainLoop();
    return 0;
}
