#ifdef __APPLE__
#include <GLUT/glut.h> // macOS (system GLUT)
#else
#include <GL/freeglut.h> // Linux/Windows (FreeGLUT)
#endif

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_TRIANGLES); // immediate mode
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(-0.5f, -0.5f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(0.5f, -0.5f);
    glColor3f(0.0f, 0.5f, 1.0f);
    glVertex2f(0.0f, 0.5f);
    glEnd();

    glutSwapBuffers(); // double-buffered swap
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // double buffering + RGB
    glutInitWindowSize(640, 480);
    glutCreateWindow("Triangle (Immediate Mode)");

    glClearColor(0.f, 0.f, 0.f, 1.f); // black background

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
