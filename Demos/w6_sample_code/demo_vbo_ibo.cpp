#include <GL/glew.h>
#include <cstdio>
#include <cstdlib>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

GLuint vbo = 0;
GLuint ibo = 0;

void initBuffers()
{
    // Vertex data: x, y, z | r, g, b
    const GLfloat vertices[] = {
        // position          // color
        -0.5f, -0.5f, 0.f, 1.f, 0.f, 0.f, // bottom-left
        0.5f, -0.5f, 0.f, 0.f, 1.f, 0.f,  // bottom-right
        0.5f, 0.5f, 0.f, 0.f, 0.f, 1.f,   // top-right
        -0.5f, 0.5f, 0.f, 1.f, 1.f, 0.f   // top-left
    };

    // Index data (two triangles forming a rectangle)
    const GLuint indices[] = {
        0, 1, 2, // first triangle
        2, 3, 0  // second triangle
    };

    // --- VBO ---
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // --- IBO ---
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Unbind for safety
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    const GLsizei stride = 6 * sizeof(GLfloat);
    glVertexPointer(3, GL_FLOAT, stride, (void *)0);
    glColorPointer(3, GL_FLOAT, stride, (void *)(3 * sizeof(GLfloat)));

    // Draw rectangle using indices
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)0);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glutSwapBuffers();
}

void cleanup()
{
    if (vbo)
        glDeleteBuffers(1, &vbo);
    if (ibo)
        glDeleteBuffers(1, &ibo);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutCreateWindow("VBO + IBO");

    glClearColor(0.f, 0.f, 0.f, 1.f);

    glewInit();
    initBuffers();
    atexit(cleanup);

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
