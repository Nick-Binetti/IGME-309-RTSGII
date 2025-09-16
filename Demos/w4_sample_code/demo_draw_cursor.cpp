#ifdef __APPLE__
#include <GLUT/glut.h> // include glut for Mac
#else
#include <GL/freeglut.h> //include glut for Windows
#endif

#include <iostream>

// Window/raster size
int rasterWidth = 800;
int rasterHeight = 600;

// Canvas size (logical coordinates)
float canvasWidth = 2.0f;  // from -1.0 to +1.0
float canvasHeight = 2.0f; // from -1.0 to +1.0

// Mouse position in canvas coordinates
float mousePos[2] = {0.0f, 0.0f};

void drawCursor()
{
    glColor3f(1.0f, 0.0f, 1.0f); // magenta
    glPointSize(10.0f);
    glBegin(GL_POINTS);
    glVertex2f(mousePos[0], mousePos[1]);
    glEnd();
    glPointSize(1.0f);
}

void motion(int x, int y)
{
    // Convert raster coords to canvas coords
    mousePos[0] = (float)x / rasterWidth * canvasWidth - canvasWidth / 2.0f;
    mousePos[1] = (float)(rasterHeight - y) / rasterHeight * canvasHeight - canvasHeight / 2.0f;

    glutPostRedisplay(); // ask GLUT to redraw
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    drawCursor();

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    rasterWidth = w;
    rasterHeight = h;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-canvasWidth / 2.0f, canvasWidth / 2.0f,
               -canvasHeight / 2.0f, canvasHeight / 2.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(rasterWidth, rasterHeight);
    glutCreateWindow("Cursor Tracking Example");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMotionFunc(motion);        // motion with button pressed
    glutPassiveMotionFunc(motion); // motion with no button pressed

    glutMainLoop();
    return 0;
}
