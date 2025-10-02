// displaylist_plain.cpp
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

GLuint baseID; // base for our 2 display lists: baseID (triangle), baseID+1 (square)

void initDisplayLists()
{
    baseID = glGenLists(2);

    // Triangle
    glNewList(baseID, GL_COMPILE);
    glBegin(GL_TRIANGLES);
    glColor3f(1, 0, 0);
    glVertex2f(0.0f, 0.5f);
    glColor3f(0, 1, 0);
    glVertex2f(-0.5f, -0.5f);
    glColor3f(0, 0, 1);
    glVertex2f(0.5f, -0.5f);
    glEnd();
    glEndList();

    // Square (centered)
    glNewList(baseID + 1, GL_COMPILE);
    glBegin(GL_QUADS);
    glColor3f(1, 1, 0);
    glVertex2f(-0.4f, -0.4f);
    glColor3f(0, 1, 1);
    glVertex2f(-0.4f, 0.4f);
    glColor3f(1, 0, 1);
    glVertex2f(0.4f, 0.4f);
    glColor3f(0.5f, 0.5f, 0.5f);
    glVertex2f(0.4f, -0.4f);
    glEnd();
    glEndList();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // No transforms—just call the lists
    glCallList(baseID);     // triangle
    glCallList(baseID + 1); // square
    glCallList(baseID);     // triangle

    glutSwapBuffers(); // double buffering
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Display Lists Reusable");

    initDisplayLists();
    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}
