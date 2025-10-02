// triangle_vertexarray.cpp
#ifdef __APPLE__
#include <GLUT/glut.h> // macOS
#else
#include <GL/freeglut.h> // Linux/Windows
#endif

// Vertex positions (x, y)
GLfloat vertices[][2] = {
    {-0.5f, -0.5f},
    {0.5f, -0.5f},
    {0.0f, 0.5f}};

// Vertex colors (r, g, b)
GLfloat colors[][3] = {
    {1.0f, 0.0f, 0.0f}, // red
    {0.0f, 1.0f, 0.0f}, // green
    {0.0f, 0.5f, 1.0f}  // blue-ish
};

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // enable and point to vertex arrays
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glColorPointer(3, GL_FLOAT, 0, colors);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    glutSwapBuffers();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Triangle with Vertex Array");

    glClearColor(0.f, 0.f, 0.f, 1.f);

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
