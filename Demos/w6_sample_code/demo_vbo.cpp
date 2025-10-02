#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h> // macOS system GLUT
#else
#include <GL/freeglut.h> // Linux/Windows (FreeGLUT)
#endif

#include <cstdio>
#include <cstdlib>

GLuint vbo = 0;

void initVBO()
{
    // Interleaved vertex data: x y z  r g b
    const GLfloat verts[] = {
        //   position           color
        -0.5f, -0.5f, 0.0f, 1.f, 0.f, 0.f,
        0.5f, -0.5f, 0.0f, 0.f, 1.f, 0.f,
        0.0f, 0.5f, 0.0f, 0.f, 0.5f, 1.f};

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo); // make `vbo` the *current* GL_ARRAY_BUFFER
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Binding to 0 (the default/null buffer)
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    // Your buffer holds 6 floats per vertex:
    // [x y z | r g b]  [x y z | r g b]  [x y z | r g b] ...
    const GLsizei stride = 6 * sizeof(GLfloat);
    glVertexPointer(3, GL_FLOAT, stride, (void *)0);
    glColorPointer(3, GL_FLOAT, stride, (void *)(3 * sizeof(GLfloat)));

    // 3 → use 3 components per vertex (x, y, z).
    // GL_FLOAT → each component is a float.
    // stride → move 24 bytes forward to get to the next vertex position.
    // (void)0* → the positions start at the beginning of each record (offset = 0).

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glutSwapBuffers();
}

void cleanup()
{
    if (vbo)
        glDeleteBuffers(1, &vbo);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutCreateWindow("VBO");

    glClearColor(0.f, 0.f, 0.f, 1.f);

    glewInit();
    initVBO();
    atexit(cleanup);

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
