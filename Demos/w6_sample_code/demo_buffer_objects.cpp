// This is an example demonstrating how to use Buffer Objects
//  regarding the data format for vertices and indices, pelase refer to the lecture slides about drawing methods.
#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <iostream>
#include <math.h>
using namespace std;

int win_width = 600, win_height = 600;
float canvas_width = 10.0f, canvas_height = 10.0f;

float curMouse[2];
float preMouse[2];

float *vertices;
unsigned short *indices;

float *colors;

unsigned int vbo_id; // vertex buffer id
unsigned int cbo_id; // color buffer id
unsigned int ibo_id; // index buffer id

void prepareBufObjects()
{
    // prepare VBO
    // generate a new VBO and get the associated ID
    glGenBuffers(1, &vbo_id);

    // bind VBO in order to use
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);

    // upload data to VBO - data went to GPU
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 5 * 3, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // clean up

    // it is safe to delete after copying data to VBO
    // for example, if vertices array was dynamically created, it is saft to call "delete [ ] vertices;"
    delete[] vertices;

    // repeat for colors and indices

    glGenBuffers(1, &cbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, cbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 5 * 3, colors, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // clean up
    delete[] colors;

    glGenBuffers(1, &ibo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * 3 * 3, indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // clean up
    delete[] indices;

    // by now, we moved the position and index data to GPU. There will be no redundant data copy at drawing time
}

void init(void)
{
    // create verices
    vertices = new float[5 * 3]; // 5 vertices and 3 channels (xyz) per vertex
    vertices[0] = 2.0f;
    vertices[1] = 4.0f;
    vertices[2] = 0.0f;
    vertices[3] = 4.0f;
    vertices[4] = 8.0f;
    vertices[5] = 0.0f,
    vertices[6] = 6.0f;
    vertices[7] = 6.0f;
    vertices[8] = 0.0f,
    vertices[9] = 7.0f;
    vertices[10] = 3.0f;
    vertices[11] = 0.0f,
    vertices[12] = 5.0f;
    vertices[13] = 3.0f;
    vertices[14] = 0.0f;

    colors = new float[5 * 3]; // per vertex color
    colors[0] = 1.0f;
    colors[1] = 0.0f;
    colors[2] = 0.0f;
    colors[3] = 0.0f;
    colors[4] = 1.0f;
    colors[5] = 0.0f;
    colors[6] = 0.0f;
    colors[7] = 0.0f;
    colors[8] = 1.0f;
    colors[9] = 1.0f;
    colors[10] = 1.0f;
    colors[11] = 0.0f;
    colors[12] = 1.0f;
    colors[13] = 0.0f;
    colors[14] = 1.0f;

    indices = new unsigned short[3 * 3]; // 3 triangles and 3 channels (v1, v2 ,v3) per triangle
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 0;
    indices[4] = 2;
    indices[5] = 4;
    indices[6] = 4;
    indices[7] = 2;
    indices[8] = 3;
}

void display(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, cbo_id);
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(3, GL_FLOAT, 0, NULL);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id);
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_SHORT, NULL);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    win_width = w;
    win_height = h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, canvas_width, 0, canvas_height);
    glViewport(0, 0, (GLsizei)win_width, (GLsizei)win_height);

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    if (key == 27) // 'esc' key
    {
        // Don't forget to deallocate the buffer objects from the GPU when your program is shut down!
        glDeleteBuffers(1, &vbo_id);
        glDeleteBuffers(1, &cbo_id);
        glDeleteBuffers(1, &ibo_id);

        exit(0);
    }
    glutPostRedisplay();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(win_width, win_height);
    glutCreateWindow("VBO Example");

    glewInit();
    init();
    prepareBufObjects();
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}