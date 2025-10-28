#include <GL/glut.h>
#include <cmath>
#include <iostream>

#ifndef M_PI
#define M_PI 3.1415926f
#endif

// Camera control
float camAngleX = 30.0f;
float camAngleY = 45.0f;
float camDistance = 5.0f;
int lastMouseX, lastMouseY;
bool mouseDown = false;

bool transformationApplied = false;

void drawText(float x, float y, const char *text)
{
    glRasterPos2f(x, y);
    while (*text)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *text);
        text++;
    }
}

void drawAxes()
{
    glLineWidth(2.0f);
    glBegin(GL_LINES);

    // X-axis (Red)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(1.5f, 0.0f, 0.0f);

    // Y-axis (Green)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 1.5f, 0.0f);

    // Z-axis (Blue)
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 1.5f);

    glEnd();
    glLineWidth(1.0f);

    // Axis labels
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos3f(1.6f, 0.0f, 0.0f);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'X');
    glRasterPos3f(0.0f, 1.6f, 0.0f);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'Y');
    glRasterPos3f(0.0f, 0.0f, 1.6f);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'Z');
}

void drawCube()
{
    glBegin(GL_QUADS);

    // +X face - Bright Red
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, -0.5f); // UE: (0.5, -0.5, -0.5)
    glVertex3f(0.5f, 0.5f, -0.5f);  // UE: (0.5,  0.5, -0.5)
    glVertex3f(0.5f, 0.5f, 0.5f);   // UE: (0.5,  0.5,  0.5)
    glVertex3f(0.5f, -0.5f, 0.5f);  // UE: (0.5, -0.5,  0.5)

    // -X face - Dark Red
    glColor3f(0.3f, 0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);

    // +Y face - Bright Green
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-0.5f, 0.5f, -0.5f); // UE: (-0.5, 0.5, -0.5)
    glVertex3f(-0.5f, 0.5f, 0.5f);  // UE: (-0.5, 0.5,  0.5)
    glVertex3f(0.5f, 0.5f, 0.5f);   // UE: ( 0.5, 0.5,  0.5)
    glVertex3f(0.5f, 0.5f, -0.5f);  // UE: ( 0.5, 0.5, -0.5)

    // -Y face - Dark Green
    glColor3f(0.0f, 0.3f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);

    // +Z face - Bright Blue
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f); // UE: (-0.5, -0.5, 0.5)
    glVertex3f(0.5f, -0.5f, 0.5f);  // UE: ( 0.5, -0.5, 0.5)
    glVertex3f(0.5f, 0.5f, 0.5f);   // UE: ( 0.5,  0.5, 0.5)
    glVertex3f(-0.5f, 0.5f, 0.5f);  // UE: (-0.5,  0.5, 0.5)

    // -Z face - Dark Blue
    glColor3f(0.0f, 0.0f, 0.3f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);

    glEnd();
}

void applyTransformation()
{
    /**************************************************/
    // Write your code here.
    //
    // Case 1: Imperial Unit System to Metric Unit System
    //glScalef(1 / 2.54, 1 / 2.54, 1 / 2.54);
    // Case 2: Y-up to Y-down
    //glRotatef(90, 0, 1, 0);
    //glRotatef(180, 1, 0, 1);
    // Case 3: Y-up to Z-up
    //glRotatef(-90, 1, 0, 0);
    // Case 4: Right hand Y-up to left hand Y-up
    //glScalef(1, 1, -1);
    // Case 5: Right hand Y-up to Left hand Z-up
   /* glRotatef(-90, 1, 0, 0);
    glRotatef(09 - , 0, 0, 1);*/
    // Case 6: Right hand Y-up to Left hand X-up
    /*glRotatef(90, 0, 0, 1);
    glRotatef(90, 1, 0, 0);*/
    /**************************************************/
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw 2D status UI
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);

    if (transformationApplied)
    {
        glColor3f(0.0f, 1.0f, 0.0f);
        drawText(10, 560, "TRANSFORMATION APPLIED");
    }
    else
    {
        glColor3f(1.0f, 0.3f, 0.0f);
        drawText(10, 560, "NO TRANSFORMATION");
    }

    glColor3f(0.7f, 0.7f, 0.7f);
    drawText(10, 580, "SPACE: Toggle Transformation | R: Reset | Mouse: Rotate View");

    // Draw 3D scene
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Position camera
    float camX = camDistance * sin(camAngleY * M_PI / 180.0f) * cos(camAngleX * M_PI / 180.0f);
    float camY = camDistance * sin(camAngleX * M_PI / 180.0f);
    float camZ = camDistance * cos(camAngleY * M_PI / 180.0f) * cos(camAngleX * M_PI / 180.0f);

    gluLookAt(camX, camY, camZ, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    // Draw with transformation
    glPushMatrix();
    if (transformationApplied)
        applyTransformation();
    drawAxes();
    drawCube();
    glPopMatrix();

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
}

void keyboard(unsigned char key, int x, int y)
{
    if (key == ' ')
    {
        transformationApplied = !transformationApplied;
        std::cout << "Transformation " << (transformationApplied ? "APPLIED" : "OFF") << std::endl;
        glutPostRedisplay();
    }
    else if (key == 'r' || key == 'R')
    {
        transformationApplied = false;
        std::cout << "Reset" << std::endl;
        glutPostRedisplay();
    }
    else if (key == 27)
    {
        exit(0);
    }
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            mouseDown = true;
            lastMouseX = x;
            lastMouseY = y;
        }
        else
        {
            mouseDown = false;
        }
    }
}

void motion(int x, int y)
{
    if (mouseDown)
    {
        float dx = x - lastMouseX;
        float dy = y - lastMouseY;

        camAngleY += dx * 0.5f;
        camAngleX += dy * 0.5f;

        if (camAngleX > 89.0f)
            camAngleX = 89.0f;
        if (camAngleX < -89.0f)
            camAngleX = -89.0f;

        lastMouseX = x;
        lastMouseY = y;

        glutPostRedisplay();
    }
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Coordinate System Transformation Practice");

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    glutMainLoop();
    return 0;
}