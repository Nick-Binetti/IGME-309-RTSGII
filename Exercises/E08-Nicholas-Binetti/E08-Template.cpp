#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#define _USE_MATH_DEFINES
#include <ctime>
#include <cmath>

using namespace std;

float canvasSize[] = { 20.0f, 20.0f };
int rasterSize[] = { 600, 600 };

// timing variables
unsigned int curTime = 0;
unsigned int preTime = 0;
float deltaTime = 0;

// pendulum motion
float rotationAngle = 0.0f;
float rotateSpeed = 90.0f; // degrees per second

// pendulum parameters
float anchorX = 10.0f;
float anchorY = 15.0f;
float stringLength = 7.0f;
float ballRadius = 1.0f;

// helper function to draw a filled circle
void drawCircle(float cx, float cy, float r, int segments) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= segments; i++) {
        float angle = 2.0f * M_PI * i / segments;
        float x = cx + r * cos(angle);
        float y = cy + r * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
}

void display(void)
{
    glClearColor(0.9f, 0.9f, 0.7f, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /*****************************/
    // write your code below
  
    // Draw anchor point
    glPointSize(10.0f);
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_POINTS);
    glVertex2f(anchorX, anchorY);
    glEnd();

    // Apply rotation around the anchor point
    glPushMatrix(); //saves matrix(transformations)
    glTranslatef(anchorX, anchorY, 0.0f);      // move origin to anchor
    glRotatef(rotationAngle, 0.0f, 0.0f, 1.0f); // rotate around z-axis
    glTranslatef(0.0f, -stringLength, 0.0f);   // move to pendulum ball position

    // Draw ball
    glColor3f(0.8f, 0.1f, 0.1f);
    drawCircle(0.0f, 0.0f, ballRadius, 40);

    // Draw string
    glColor3f(0.4f, 0.2f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(0.0f, stringLength); // top (back to anchor)
    glVertex2f(0.0f, 0.0f);         // bottom (center of ball)
    glEnd();

    

    glPopMatrix(); //restores matrix(transformations

    // write your code above
    /*****************************/

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    rasterSize[0] = w;
    rasterSize[1] = h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, canvasSize[0], 0, canvasSize[1]);
    glViewport(0, 0, rasterSize[0], rasterSize[1]);

    glutPostRedisplay();
}

void update() {
    curTime = glutGet(GLUT_ELAPSED_TIME);
    deltaTime = (float)(curTime - preTime) / 1000.0f; // convert ms to seconds

    if (rotationAngle > 45.0f) {
        rotateSpeed = -abs(rotateSpeed);
    }
    else if (rotationAngle < -45.0f) {
        rotateSpeed = abs(rotateSpeed);
    }
    rotationAngle += deltaTime * rotateSpeed;

    preTime = curTime;
    glutPostRedisplay();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(rasterSize[0], rasterSize[1]);
    glutCreateWindow("Simple Pendulum");

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutIdleFunc(update);

    preTime = glutGet(GLUT_ELAPSED_TIME);
    glutMainLoop();
    return 0;
}
