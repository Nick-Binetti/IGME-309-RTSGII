#ifdef __APPLE__
#include <GLUT/glut.h> // macOS
#else
#include <GL/freeglut.h> // Linux/Windows
#endif

GLuint triangleList; // display list ID

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glCallList(triangleList); // draw from display list

    glutSwapBuffers(); // double-buffered swap
}

void initDisplayList()
{
    triangleList = glGenLists(1);
    glNewList(triangleList, GL_COMPILE);
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(-0.5f, -0.5f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(0.5f, -0.5f);
    glColor3f(0.0f, 0.5f, 1.0f);
    glVertex2f(0.0f, 0.5f);
    glEnd();
    glEndList();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Triangle with Display List");

    glClearColor(0.f, 0.f, 0.f, 1.f);

    initDisplayList();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
