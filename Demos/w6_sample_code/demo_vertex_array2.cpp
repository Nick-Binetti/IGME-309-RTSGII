// This is an example demonstrating how to use Vertex Array method
//  regarding the data format for vertices and indices, pelase refer to the lecture note about drawing methods.

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

float vertices[] = {2.0f, 4.0f, 0.0f,
                    4.0f, 8.0f, 0.0f,
                    6.0f, 6.0f, 0.0f,
                    7.0f, 3.0f, 0.0f,
                    5.0f, 3.0f, 0.0f};
unsigned short indices[] = {0, 1, 2,
                            0, 2, 4,
                            4, 2, 3};

float colors[] = {1.0f, 0.0f, 0.0f, // per vertex color
                  0.0f, 1.0f, 0.0f,
                  0.0f, 0.0f, 1.0f,
                  1.0f, 1.0f, 0.0f,
                  1.0f, 0.0f, 1.0f};

void init(void)
{
}

void display(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // enble and specify pointers to vertex arrays
    // draw triangles using glDrawElements
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);

    glColorPointer(3, GL_FLOAT, 0, colors);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_SHORT, indices);

    glDisableClientState(GL_VERTEX_ARRAY); // disable vertex arrays
    glDisableClientState(GL_COLOR_ARRAY);

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
        exit(0);
    glutPostRedisplay();
}

int main(int argc, char *argv[])
{
    init();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(win_width, win_height);
    glutCreateWindow("Vertex Array Drawing Method");

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}