#ifdef __APPLE__
#include <GLUT/glut.h> // include glut for Mac
#else
#include <GL/freeglut.h> //include glut for Windows
#endif

float color = 0.0f;
bool useDouble = true; // toggle between single and double

void display()
{
    glClearColor(color, 0.0, 1.0 - color, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.5f, -0.5f);
    glVertex2f(0.5f, 0.5f);
    glVertex2f(-0.5f, 0.5f);
    glEnd();

    if (useDouble)
    {
        glutSwapBuffers(); // present back buffer (double)
    }
    else
    {
        glFlush(); // force draw immediately (single)
    }
}

void timer(int)
{
    color += 0.02f;
    if (color > 1.0f)
        color = 0.0f;
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    if (useDouble)
    {
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    }
    else
    {
        glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    }

    glutInitWindowSize(400, 400);
    glutCreateWindow(useDouble ? "Double Buffering" : "Single Buffering");

    glutDisplayFunc(display);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}
