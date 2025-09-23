#include <GL/freeglut.h> //include glut for Windows
#include <cmath>

//global variables
int width;
int height;
int vertNum = 30;

void drawFilledCircle(float red, float green, float blue, float center_x, float center_y, float radius)
{
    glColor3f(red, green, blue);
    // draw the cicle
    glBegin(GL_POLYGON); // filled
    for (int i = 0; i < vertNum; ++i)
    {
        float t = (float)i / vertNum * 2.0f * 3.14f;
        glVertex2f(center_x + radius * cos(t), center_y + radius * sin(t));
    }
    glEnd();
}

void drawWireframeCircle(float red, float green, float blue, float center_x, float center_y, float radius, float lineWidth)
{
    glColor3f(red, green, blue);
    // draw the cicle
    glBegin(GL_LINE_LOOP); //wireframe
    for (int i = 0; i < vertNum; ++i)
    {
        float t = (float)i / vertNum * 2.0f * 3.14f;
        glVertex2f(center_x + radius * cos(t), center_y + radius * sin(t));
    }
    glEnd();
}

void reshape(int w, int h)
{
    width = w;
    height = h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, width, 0.0, height);
    glViewport(0, 0, width, height);

}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27:
            exit(0);
            break;
        case 43:
            vertNum++;
            break;
        case 45:
            vertNum--;
            break;
    }
}

void update() 
{
    //redraws circles so that in the case of num vertices change the user will see it instantly
    glutPostRedisplay();
}

// called when the GL context need to be rendered
void display(void)
{

    // clear the screen to white, which is the background color
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //body
    drawFilledCircle(0.0f, 0.0f, 0.0f, width / 2, height / 2, 100.0f);

    glutSwapBuffers();

}

int main(int argc, char* argv[])
{
    //initialize GLUT, let it extract command-line GLUT options that you may provide
    //NOTE that the '&' before argc
    glutInit(&argc, argv);

    // specify as double bufferred can make the display faster
    // Color is speicfied to RGBA, four color channels with Red, Green, Blue and Alpha(depth)
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    //set the initial window size */
    glutInitWindowSize(width,height);

    // create the window with a title
    glutCreateWindow("Panda");

    glutDisplayFunc(display); 
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(update);



    //start the glut main loop
    glutMainLoop();

    return 0;
}