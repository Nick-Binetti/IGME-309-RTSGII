#ifdef __APPLE__
#include <GLUT/glut.h> // macOS
#else
#include <GL/freeglut.h> // Linux/Windows
#endif

// Simple init
void init()
{
    glClearColor(1.0, 1.0, 1.0, 1.0); // white background
}

// Draw callback
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // --- First: independent triangles (6 vertices for a rectangle) ---
    glColor3f(1.0f, 0.0f, 0.0f); // red
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.9f, -0.5f); // first triangle
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(-0.9f, 0.0f);

    glVertex2f(-0.9f, 0.0f); // second triangle
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(-0.5f, 0.0f);
    glEnd();

    // --- Second: triangle strip (4 vertices for same rectangle) ---
    glColor3f(0.0f, 0.0f, 1.0f); // blue
    glBegin(GL_TRIANGLE_STRIP);
    glVertex2f(0.1f, -0.5f); // v1
    glVertex2f(0.5f, -0.5f); // v2
    glVertex2f(0.1f, 0.0f);  // v3
    glVertex2f(0.5f, 0.0f);  // v4
    glEnd();

    glutSwapBuffers();
}

// Main
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("GL_TRIANGLES vs GL_TRIANGLE_STRIP");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
