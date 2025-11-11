#include <GL/glut.h>
#include <cmath>
#include <iostream>

// Cubic Bézier function
float cubicBezier(float t, float p0, float p1, float p2, float p3)
{
    float u = 1.0f - t;
    return u * u * u * p0 + 3 * u * u * t * p1 + 3 * u * t * t * p2 + t * t * t * p3;
}

// Solve for t given x using Newton-Raphson method
float solveBezierX(float x, float x1, float x2)
{
    float t = x;
    for (int i = 0; i < 8; i++)
    {
        float xt = cubicBezier(t, 0.0f, x1, x2, 1.0f);
        float dx = xt - x;
        if (fabs(dx) < 0.0001f)
            break;

        float u = 1.0f - t;
        float derivative = 3 * u * u * (x1 - 0.0f) + 6 * u * t * (x2 - x1) + 3 * t * t * (1.0f - x2);
        if (fabs(derivative) < 0.000001f)
            break;

        t -= dx / derivative;
        t = fmax(0.0f, fmin(1.0f, t));
    }
    return t;
}

// Cubic-bezier function
float cubicBezierEasing(float x, float x1, float y1, float x2, float y2)
{
    if (x <= 0.0f)
        return 0.0f;
    if (x >= 1.0f)
        return 1.0f;

    float t = solveBezierX(x, x1, x2);
    return cubicBezier(t, 0.0f, y1, y2, 1.0f);
}

// Animation state
float animTime = 0.0f;
float animDuration = 2.0f;
bool animReverse = false;

void drawBox(float size)
{
    glBegin(GL_QUADS);
    glVertex2f(-size, -size);
    glVertex2f(size, -size);
    glVertex2f(size, size);
    glVertex2f(-size, size);
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    float progress = animTime / animDuration;
    if (animReverse)
        progress = 1.0f - progress;

    // Box 1: Translation (Top-Left) - Red
    // Using ease-in-out: cubic-bezier(0.42, 0, 0.58, 1)
    glPushMatrix();
    float easeTranslate = cubicBezierEasing(progress, 0.42f, 0.0f, 0.58f, 1.0f);
    float translateX = -0.5f - 0.25 + easeTranslate * 0.6f;
    glTranslatef(translateX, 0.5f, 0.0f);
    glColor3f(1.0f, 0.0f, 0.0f); // Red
    drawBox(0.15f);
    glPopMatrix();

    // Box 2: Rotation (Top-Right) - Green
    // Using ease-out: cubic-bezier(0, 0, 0.58, 1)
    glPushMatrix();
    glTranslatef(0.5f, 0.5f, 0.0f); // position the box to top right.
    // ****************************************************
    // Write your code here.
    float easeRotate = cubicBezierEasing(progress, 0.0f, 0.0f, 0.58f, 1.0f);
    float rotateX =  easeRotate * 360.0f;
    glRotatef(rotateX, 0.0f, 0.0f, 1.0f);
    glColor3f(0.0f, 1.0f, 0.0f); // Red
    drawBox(0.15f);
    //
    // ****************************************************
    glPopMatrix();

    // Box 3: Scale (Bottom-Left) - Blue
    // Using ease-in: cubic-bezier(0.42, 0, 1, 1)
    glPushMatrix();
    glTranslatef(-0.5f, -0.5f, 0.0f); // position the box to bottom left.
    // ****************************************************
    // Write your code here.
    float easeScale = cubicBezierEasing(progress, 0.42f, 0.0f, 1.0f, 1.0f);
    float scaleX = 5 + easeScale * 2;
    glScalef(scaleX, 0.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f); // Red
    drawBox(0.15f);
    // ****************************************************
    glPopMatrix();

    // Box 4: Color (Bottom-Right) - Blue to Green
    // Using custom: cubic-bezier(0.68, -0.55, 0.265, 1.55)
    glPushMatrix();
    glTranslatef(0.5f, -0.5f, 0.0f); // position the box to bottom right.
    // ****************************************************
    // Write your code here.
    //
    // ****************************************************
    glPopMatrix();

    glutSwapBuffers();
}

void timer(int value)
{
    animTime += 0.016f; // ~60 FPS

    if (animTime >= animDuration)
    {
        animTime = 0.0f;
        animReverse = !animReverse;
    }

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void init()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Bezier Curve Animations");

    init();

    glutDisplayFunc(display);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}
