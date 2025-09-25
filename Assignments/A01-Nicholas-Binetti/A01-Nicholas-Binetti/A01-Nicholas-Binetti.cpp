#include <GL/freeglut.h> //include glut for Windows
#include <cmath>

//global variables
int screenWidth;
int screenHeight;
int vertNum = 30;

void drawFilledCircle(float red, float green, float blue, float center_x, float center_y, float radius)
{
    glColor3f(red, green, blue);
    // draw the cicle by looping through printing each vertex to the screen
    glBegin(GL_POLYGON); // filled
    for (int i = 0; i < vertNum; ++i)
    {
        //circle algo
        float t = (float)i / vertNum * 2.0f * 3.14f;
        glVertex2f(center_x + radius * cos(t), center_y + radius * sin(t));
    }
    glEnd();
}

void drawWireframeCircle(float red, float green, float blue, float center_x, float center_y, float radius, float lineWidth)
{
    glColor3f(red, green, blue);
    
    //change linewidth according to parameter

    glLineWidth(lineWidth);
    // draw the cicle by looping through printing each vertex to the screen
    glBegin(GL_LINE_LOOP); //wireframe
    for (int i = 0; i < vertNum; ++i)
    {
        //circle algo
        float t = (float)i / vertNum * 2.0f * 3.14f;
        glVertex2f(center_x + radius * cos(t), center_y + radius * sin(t));
    }
    glEnd();
}

void reshape(int w, int h)
{
    screenWidth = w;
    screenHeight = h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, screenWidth, 0.0, screenHeight);
    glViewport(0, 0, screenWidth, screenHeight);

}

//for keyboard events(esc, +, -)
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
    glClearColor(0.5, 0.85, 0.5, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //Body
    drawFilledCircle(0.0f, 0.0f, 0.0f, screenWidth / 2, screenHeight / 2, 100.0f);
    drawFilledCircle(1.0f, 1.0f, 1.0f, screenWidth / 2, screenHeight / 2, 50.0f);

    //Left paws 
    drawFilledCircle(0.0f, 0.0f, 0.0f, screenWidth / 2.5, screenHeight / 1.75, 50.0f);
    drawFilledCircle(0.0f, 0.0f, 0.0f, screenWidth / 2.33, screenHeight / 2.75, 50.0f);

    drawFilledCircle(1.0f, 1.0f, 1.0f, screenWidth / 2.5, screenHeight / 1.75, 20.0f);
    drawFilledCircle(1.0f, 1.0f, 1.0f, screenWidth / 2.33, screenHeight / 2.75, 20.0f);

    drawFilledCircle(1.0f, 1.0f, 1.0f, screenWidth / 2.61, screenHeight / 1.65, 10.0f);
    drawFilledCircle(1.0f, 1.0f, 1.0f, screenWidth / 2.5, screenHeight / 1.63, 10.0f);
    drawFilledCircle(1.0f, 1.0f, 1.0f, screenWidth / 2.4, screenHeight / 1.65, 10.0f);

    drawFilledCircle(1.0f, 1.0f, 1.0f, screenWidth / 2.43, screenHeight / 2.5, 10.0f);
    drawFilledCircle(1.0f, 1.0f, 1.0f, screenWidth / 2.33, screenHeight / 2.45, 10.0f);
    drawFilledCircle(1.0f, 1.0f, 1.0f, screenWidth / 2.25, screenHeight / 2.5, 10.0f);

    //Right paws
    drawFilledCircle(0.0f, 0.0f, 0.0f, screenWidth / 1.67, screenHeight / 1.75, 50.0f);
    drawFilledCircle(0.0f, 0.0f, 0.0f, screenWidth / 1.75, screenHeight / 2.75, 50.0f);

    drawFilledCircle(1.0f, 1.0f, 1.0f, screenWidth / 1.67, screenHeight / 1.75, 20.0f);
    drawFilledCircle(1.0f, 1.0f, 1.0f, screenWidth / 1.75, screenHeight / 2.75, 20.0f);

    drawFilledCircle(1.0f, 1.0f, 1.0f, screenWidth / 1.72, screenHeight / 1.65, 10.0f);
    drawFilledCircle(1.0f, 1.0f, 1.0f, screenWidth / 1.67, screenHeight / 1.62, 10.0f);
    drawFilledCircle(1.0f, 1.0f, 1.0f, screenWidth / 1.62, screenHeight / 1.65, 10.0f);

    drawFilledCircle(1.0f, 1.0f, 1.0f, screenWidth / 1.8, screenHeight / 2.5, 10.0f);
    drawFilledCircle(1.0f, 1.0f, 1.0f, screenWidth / 1.75, screenHeight / 2.45, 10.0f);
    drawFilledCircle(1.0f, 1.0f, 1.0f, screenWidth / 1.7, screenHeight / 2.5, 10.0f);

    //Ears
    drawFilledCircle(0.0f, 0.0f, 0.0f, screenWidth / 2.15, screenHeight / 1.2, 25.0f);
    drawFilledCircle(0.0f, 0.0f, 0.0f, screenWidth / 1.86, screenHeight / 1.2, 25.0f);

    drawFilledCircle(1.0f, 0.6f, 0.7f, screenWidth / 2.15, screenHeight / 1.2, 10.0f);
    drawFilledCircle(1.0f, 0.6f, 0.7f, screenWidth / 1.86, screenHeight / 1.2, 10.0f);

    //Head
    drawFilledCircle(1.0f, 1.0f, 1.0f, screenWidth / 2, screenHeight/1.35, 75.0f);

    //-eyes
    drawWireframeCircle(0.0f, 0.0f, 0.0f, screenWidth / 2.09, screenHeight / 1.3, 20.0f, 10);
    drawWireframeCircle(0.0f, 0.0f, 0.0f, screenWidth / 1.91, screenHeight / 1.3, 20.0f, 2);
    //-pupils
    drawFilledCircle(0.0f, 0.0f, 0.0f, screenWidth / 2.05, screenHeight / 1.3, 10.0f);
    drawFilledCircle(0.0f, 0.0f, 0.0f, screenWidth / 1.94, screenHeight / 1.3, 10.0f);
    //-mouth
    drawWireframeCircle(0.0f, 0.0f, 0.0f, screenWidth / 2, screenHeight / 1.5, 16.0f, 2);
    drawFilledCircle(1.0f, 0.6f, 0.7f, screenWidth / 2, screenHeight / 1.5, 15.0f);
    drawFilledCircle(1.0f, 0.0f, 0.0f, screenWidth / 1.98, screenHeight / 1.52, 7.0f);

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
    glutInitWindowSize(screenWidth,screenHeight);

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