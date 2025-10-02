// vbo_with_shader.cpp
#include <cstdio>
#include <cstdlib>

#include <GL/glew.h> // GLEW must be included before GLUT
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

static GLuint prog = 0;
static GLuint vao = 0;
static GLuint vbo = 0;

static const char *kVS = R"(#version 120
attribute vec3 aPos;
attribute vec3 aCol;
varying   vec3 vCol;
void main() {
    vCol = aCol;
    gl_Position = vec4(aPos, 1.0);
}
)";

static const char *kFS = R"(#version 120
varying vec3 vCol;
void main() {
    gl_FragColor = vec4(vCol, 1.0);
}
)";

GLuint compileShader(GLenum type, const char *src)
{
    GLuint s = glCreateShader(type);
    glShaderSource(s, 1, &src, nullptr);
    glCompileShader(s);
    GLint ok = 0;
    glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok)
    {
        char log[1024];
        glGetShaderInfoLog(s, sizeof(log), nullptr, log);
        std::fprintf(stderr, "Shader compile error:\n%s\n", log);
        std::exit(EXIT_FAILURE);
    }
    return s;
}

GLuint linkProgram(GLuint vs, GLuint fs)
{
    GLuint p = glCreateProgram();
    glAttachShader(p, vs);
    glAttachShader(p, fs);
    glLinkProgram(p);
    GLint ok = 0;
    glGetProgramiv(p, GL_LINK_STATUS, &ok);
    if (!ok)
    {
        char log[1024];
        glGetProgramInfoLog(p, sizeof(log), nullptr, log);
        std::fprintf(stderr, "Program link error:\n%s\n", log);
        std::exit(EXIT_FAILURE);
    }
    return p;
}

void initGL()
{
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        std::fprintf(stderr, "GLEW init failed: %s\n", glewGetErrorString(err));
        std::exit(EXIT_FAILURE);
    }

    // --- Build program ---
    GLuint vs = compileShader(GL_VERTEX_SHADER, kVS);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, kFS);
    prog = linkProgram(vs, fs);
    glDeleteShader(vs);
    glDeleteShader(fs);

    // --- Geometry (interleaved): x y z | r g b ---
    const GLfloat verts[] = {
        -0.5f, -0.5f, 0.0f, 1.f, 0.f, 0.f,
        0.5f, -0.5f, 0.0f, 0.f, 1.f, 0.f,
        0.0f, 0.5f, 0.0f, 0.f, 0.5f, 1.f};

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    GLint locPos = glGetAttribLocation(prog, "aPos");
    GLint locCol = glGetAttribLocation(prog, "aCol");

    glEnableVertexAttribArray(locPos);
    glVertexAttribPointer(locPos, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)0);

    glEnableVertexAttribArray(locCol);
    glVertexAttribPointer(locCol, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glClearColor(0.f, 0.f, 0.f, 1.f);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(prog);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    glUseProgram(0);

    glutSwapBuffers();
}

void cleanup()
{
    if (vbo)
        glDeleteBuffers(1, &vbo);
    if (vao)
        glDeleteVertexArrays(1, &vao);
    if (prog)
        glDeleteProgram(prog);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutCreateWindow("VBO + Shader");

    initGL();
    atexit(cleanup);

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
