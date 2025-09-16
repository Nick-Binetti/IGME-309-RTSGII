#ifdef __APPLE__
#include <GLUT/glut.h> // include glut for Mac
#else
#include <GL/freeglut.h> //include glut for Windows
#endif

void display()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    // wipe the entire color buffer to the current clear color.
    glClear(GL_COLOR_BUFFER_BIT);

    // Default coordinate bounds
    // x range: –1.0 (left) to +1.0 (right)
    // y range: –1.0 (bottom) to +1.0 (top)
    // z range: –1.0 (near) to +1.0 (far)

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
    glutCreateWindow("My First Triangle!");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
