
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <iostream>

#include "PolyObject.h"

using namespace std;

std::vector<PolyObject> completedPolygons;
PolyObject currentPolygon;


float canvasSize[] = {10.0f, 10.0f};
int rasterSize[] = {800, 600};

float mousePos[2];

void init(void)
{
    //initialize mousePos values
    mousePos[0] = mousePos[1] = 0.0f;
}

void drawCursor()
{
    glColor3f(1.0f, 0.0f, 1.0f);
    glPointSize(10.0f);
    glBegin(GL_POINTS);
    glVertex2fv(mousePos);
    glEnd();
    glPointSize(1.0f);
}

void display(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
   
    //draw all completed polygons in the vector
    for (PolyObject p : completedPolygons)
    {
        p.draw();
    }

    //draw incomplete/current polygon
    currentPolygon.draw(mousePos[0], mousePos[1]);
    
    drawCursor();
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    rasterSize[0] = w;
    rasterSize[1] = h;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, canvasSize[0], 0.0, canvasSize[1]);
    glViewport(0, 0, rasterSize[0], rasterSize[1]);
    
    glutPostRedisplay();
}

void mouse (int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

        mousePos[0] = (float)x / rasterSize[0] * canvasSize[0];
        mousePos[1] = (float)(rasterSize[1] - y) / rasterSize[1] * canvasSize[1];

        //add vertex @ current mouse position to current polygon
        currentPolygon.addVertex(mousePos[0], mousePos[1]);

        glutPostRedisplay();
    }
}

void motion(int x, int y)
{
    // mouse events are handled by OS, eventually. When using mouse in the raster window, it assumes top-left is the origin.
    // Note: the raster window created by GLUT assumes bottom-left is the origin.
    mousePos[0] = (float)x / rasterSize[0] * canvasSize[0];
    mousePos[1] = (float)(rasterSize[1] - y) / rasterSize[1] * canvasSize[1];
    
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    switch(key){
        //esc press
        case 27:
            exit(0);
            break;

        //enter press -> complete current polygon
        case 13:
            if (currentPolygon.getVertNum() > 0)
            {
                completedPolygons.push_back(currentPolygon);
                currentPolygon = PolyObject(); // new empty polygon
            }
            break;
    }
}

void menu(int value)
{
    switch (value) {
        case 0: // clear
            completedPolygons.clear();
            glutPostRedisplay();
            break;

        case 1: //exit
            exit(0);

        case 2: // red
            currentPolygon.setColor(1.0f, 0.0f, 0.0f);
            glutPostRedisplay();
            break;

        case 3: // green
            currentPolygon.setColor(0.0f, 1.0f, 0.0f);
            glutPostRedisplay();
            break;

        case 4: // blue
            currentPolygon.setColor(0.0f, 0.0f, 1.0f);
            glutPostRedisplay();
            break;

        case 5: // black
            currentPolygon.setColor(0.0f, 0.0f, 0.0f);
            glutPostRedisplay();

        default:
            break;
    }
}
void createMenu()
{
    int colorMenu = glutCreateMenu(menu);
    glutAddMenuEntry("Red", 2);
    glutAddMenuEntry("Green", 3);
    glutAddMenuEntry("Blue", 4);
    glutAddMenuEntry("Black", 5);
    
    glutCreateMenu(menu);
    glutAddMenuEntry("Clear", 0);
    glutAddSubMenu("Colors", colorMenu);
    glutAddMenuEntry("Exit", 1);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char *argv[])
{
    init();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(rasterSize[0], rasterSize[1]);
    glutCreateWindow("Polygon Creation Tool(PCT)");
    
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(motion);
    createMenu();
    glutMainLoop();
    return 0;
    
    
}