#ifdef __APPLE__
#include <GLUT/glut.h> // include glut for Mac
#else
#include <GL/freeglut.h> //include glut for Windows
#endif

void displayWindowA()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    // wipe the entire color buffer to the current clear color.
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0f, 0.0f, 0.0f); // red
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.5f, -0.5f);
    glVertex2f(0.0f, 0.5f);
    glEnd();

    glutSwapBuffers();
}

void displayWindowB()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    // wipe the entire color buffer to the current clear color.
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0f, 0.0f, 1.0f); // blue
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.5f, -0.5f);
    glVertex2f(0.0f, 0.5f);
    glEnd();

    glutSwapBuffers();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    glutInitWindowSize(800, 600);
    glutCreateWindow("Red Triangle");
    glutDisplayFunc(displayWindowA);

    glutInitWindowSize(600, 800);
    glutCreateWindow("Blue Triangle");
    glutDisplayFunc(displayWindowB);

    glutMainLoop();
    return 0;
}
