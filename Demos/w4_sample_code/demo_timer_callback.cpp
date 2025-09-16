#ifdef __APPLE__
#include <GLUT/glut.h> // include glut for Mac
#else
#include <GL/freeglut.h> //include glut for Windows
#endif

float x = -0.9f;  // starting position
float dx = 0.01f; // movement speed

void display()
{
    glClearColor(1.0, 1.0, 1.0, 1.0); // white background
    glClear(GL_COLOR_BUFFER_BIT);

    // draw a moving red square
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(x, -0.1f);
    glVertex2f(x + 0.2f, -0.1f);
    glVertex2f(x + 0.2f, 0.1f);
    glVertex2f(x, 0.1f);
    glEnd();

    glutSwapBuffers();
}

void timer(int v)
{
    x += dx;
    if (x > 1.0f || x < -1.0f)
        dx = -dx; // bounce

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // roughly 60 FPS
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(400, 300);
    glutCreateWindow("No glClear Example");

    glutDisplayFunc(display);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}
