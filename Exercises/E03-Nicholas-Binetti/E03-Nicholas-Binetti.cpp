#include <GL/freeglut.h> //include glut for Windows

//width and height for window size
int screenWidth = 400;
int screenHeight = 400;

// called when the GL context need to be rendered
void display(void)
{
    // clear the screen to white, which is the background color
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0f, 0.0f, 0.0f); // red
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.5f, 0.5f);   // left
    glVertex2f(0.5f, 0.5f);    // right
    glVertex2f(0.0f, -0.5f);   // bottom
    glEnd();

    glutSwapBuffers();
}

// called when window is first created or when window is resized
void reshape(int w, int h)
{
    screenWidth = w;
    screenHeight = h;

    glViewport(0, 0, screenWidth, screenHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float scale = (float)w / (float)h;

    if (scale >= 1.0f) {
        // Window is wider than tall
        gluOrtho2D(-scale, scale, -1.0, 1.0);
    }
    else {
        // Window is taller than wide
        gluOrtho2D(-1.0, 1.0, -1.0f / scale, 1.0f / scale);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(screenWidth, screenHeight);
    glutCreateWindow("First Triangle");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}
