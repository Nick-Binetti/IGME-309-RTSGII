#ifdef __APPLE__
#include <GLUT/glut.h> // macOS
#else
#include <GL/freeglut.h> // Linux/Windows
#endif

class App
{
public:
    // draw function
    void render()
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0, 0.0, 0.0);

        glBegin(GL_TRIANGLES);
        glVertex2f(-0.1f, -0.1f);
        glVertex2f(0.1f, -0.1f);
        glVertex2f(0.0f, 0.1f);
        glEnd();

        glutSwapBuffers();
    }
};

// a single global pointer to App (instead of many globals)
App *app;

void displayCB()
{
    app->render();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutCreateWindow("Class-managed FreeGLUT");

    App myApp;
    app = &myApp; // set pointer

    glutDisplayFunc(displayCB);

    glutMainLoop();
    return 0;
}