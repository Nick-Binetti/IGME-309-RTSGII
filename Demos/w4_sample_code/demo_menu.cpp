#ifdef __APPLE__
#include <GLUT/glut.h> // include glut for Mac
#else
#include <GL/freeglut.h> //include glut for Windows
#endif

#include <iostream>

// Window size
int winWidth = 500, winHeight = 500;

// Menu callback
void myMenu(int value)
{
    switch (value)
    {
    case 0: // clear
        std::cout << "Clear screen\n";
        glClear(GL_COLOR_BUFFER_BIT);
        glutSwapBuffers();
        break;

    case 1: // exit
        std::cout << "Exit program\n";
        exit(0);
        break;
    }
}

// Simple display callback
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw a white triangle for demo
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.5f, -0.5f);
    glVertex2f(0.0f, 0.5f);
    glEnd();

    glutSwapBuffers();
}

// Reshape callback
void reshape(int w, int h)
{
    winWidth = w;
    winHeight = h;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("GLUT Menu Example");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    // Create a menu and attach it to the right mouse button
    glutCreateMenu(myMenu);
    glutAddMenuEntry("Clear", 0);
    glutAddMenuEntry("Exit", 1);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();
    return 0;
}
