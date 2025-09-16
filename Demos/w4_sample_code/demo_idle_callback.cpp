#ifdef __APPLE__
#include <GLUT/glut.h> // include glut for Mac
#else
#include <GL/freeglut.h> //include glut for Windows
#endif

// Position and motion (in NDC: -1..+1)
static float x = -0.8f;         // center x
static float vx = 0.6f;         // velocity (units per second)
static const float half = 0.1f; // half-size of the square
static int lastTime = 0;        // ms

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Compute vertices directly from current x (no glTranslatef)
    float left = x - half;
    float right = x + half;
    float bottom = -half;
    float top = half;

    glColor3f(0.0f, 0.5f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(left, bottom);
    glVertex2f(right, bottom);
    glVertex2f(right, top);
    glVertex2f(left, top);
    glEnd();

    glutSwapBuffers();
}

void idle()
{
    int now = glutGet(GLUT_ELAPSED_TIME);                           // ms since glutInit
    float dt = (lastTime == 0) ? 0.0f : (now - lastTime) / 1000.0f; // seconds
    lastTime = now;

    // Update position using real-time delta
    x += vx * dt;

    // Bounce at edges (keep the square fully visible)
    if (x + half > 1.0f)
    {
        x = 1.0f - half;
        vx = -vx;
    }
    if (x - half < -1.0f)
    {
        x = -1.0f + half;
        vx = -vx;
    }

    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutCreateWindow("Translation with the idle callback");

    glClearColor(1.0, 1.0, 1.0, 1.0); // white background

    glutDisplayFunc(display);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}
