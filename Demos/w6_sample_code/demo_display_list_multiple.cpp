#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

GLuint baseID; // base of reserved list IDs

void initShapes()
{
    // Reserve 2 IDs (baseID and baseID+1)
    baseID = glGenLists(2);

    // First list: a red triangle
    glNewList(baseID, GL_COMPILE);
    glBegin(GL_TRIANGLES);
    glColor3f(1, 0, 0);
    glVertex2f(-0.5f, -0.5f);
    glColor3f(0, 1, 0);
    glVertex2f(0.5f, -0.5f);
    glColor3f(0, 0, 1);
    glVertex2f(0.0f, 0.5f);
    glEnd();
    glEndList();

    // Second list: a yellow square
    glNewList(baseID + 1, GL_COMPILE);
    glBegin(GL_QUADS);
    glColor3f(1, 1, 0);
    glVertex2f(-0.3f, -0.3f);
    glColor3f(1, 0.5f, 0);
    glVertex2f(-0.3f, 0.3f);
    glColor3f(0.5f, 1, 0.5f);
    glVertex2f(0.3f, 0.3f);
    glColor3f(0, 0.5f, 1);
    glVertex2f(0.3f, -0.3f);
    glEnd();
    glEndList();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Call the red triangle
    glPushMatrix();
    glTranslatef(-0.5f, 0.f, 0.f);
    glCallList(baseID); // triangle
    glPopMatrix();

    // Call the yellow square
    glPushMatrix();
    glTranslatef(0.5f, 0.f, 0.f);
    glCallList(baseID + 1); // rectangle
    glPopMatrix();

    glutSwapBuffers();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Multiple Display Lists Example");

    initShapes();
    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}
