#include <GL/freeglut.h> //include glut for Windows

// called when the GL context need to be rendered
void display(void)
{
    // clear the screen to white, which is the background color
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glEnd();
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
    glutInitWindowSize(640, 480);

    // create the window with a title
    glutCreateWindow("First OpenGL Program");

    //start the glut main loop
    glutMainLoop();

    return 0;
}