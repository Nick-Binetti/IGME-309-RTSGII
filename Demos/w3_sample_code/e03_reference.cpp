#include <GL/freeglut.h>

// the window's width and height
int width, height;
bool useRedColor = true;

void init(void)
{
    // initialize the size of the window
    width = 600;
    height = 600;
}

// called when window is first created or when window is resized
void reshape(int w, int h)
{
    // update thescreen dimensions
    width = w;
    height = h;

    // tells OpenGL that subsequent matrix operations will modify the projection matrix,
    // which controls how 3D coordinates are mapped to the 2D screen.
    glMatrixMode(GL_PROJECTION);
    // resets the current matrix to the identity matrix (no transformations
    glLoadIdentity();
    // sets up a 2D orthographic projection with x ranging from 0 to 10 and y ranging from 0 to 10.
    gluOrtho2D(0.0, (float)width, 0.0, (float)height);

    // defines the rectangular region of the window that OpenGL will render into.
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    // tells GLUT that the window needs to be redrawn soon
    glutPostRedisplay();
}

void display()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    // wipe the entire color buffer to the current clear color.
    glClear(GL_COLOR_BUFFER_BIT);

    const float triangleWidth = 300;
    const float triangleHeight = triangleWidth / 2.0 * 1.732;

    const float windowCenterX = (float)width * 0.5f;
    const float windowCenterY = (float)height * 0.5f;


    glColor3f(1.0f, 0.0f, 0.0f); // blue
    glBegin(GL_TRIANGLES);
    glVertex2f(windowCenterX - triangleWidth / 2.0f, windowCenterY + triangleHeight / 2.0f);
    glVertex2f(windowCenterX + triangleWidth / 2.0f, windowCenterY + triangleHeight / 2.0f);
    glVertex2f(windowCenterX, windowCenterY - triangleHeight / 2.0f);
    glEnd();

    glutSwapBuffers();
}

int main(int argc, char **argv)
{
    init();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    // set the initial window size */
    glutInitWindowSize(width, height);

    glutCreateWindow("My First Triangle!");

    glutDisplayFunc(display);

    // register function to handle window resizes
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}
