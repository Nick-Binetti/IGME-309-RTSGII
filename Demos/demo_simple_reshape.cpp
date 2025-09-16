#ifdef __APPLE__
#include <GLUT/glut.h> // include glut for Mac
#else
#include <GL/freeglut.h> //include glut for Windows
#endif

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

    // Default coordinate bounds
    // x range: –1.0 (left) to +1.0 (right)
    // y range: –1.0 (bottom) to +1.0 (top)
    // z range: –1.0 (near) to +1.0 (far)

    glColor3f(0.0f, 0.0f, 1.0f); // blue
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.5f, -0.5f);
    glVertex2f(0.0f, 0.5f);
    glEnd();

    glutSwapBuffers();
}

int main(int argc, char **argv)
{
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
