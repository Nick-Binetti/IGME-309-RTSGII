#include "sphere.h"
#include "cube.h"
#include "cone.h"
#include "torus.h"
#include "teapot.h"
#include "duck.h"
#include <vector>

// Window dimensions
const int WINDOW_WIDTH = 1400;
const int WINDOW_HEIGHT = 800;

// Collection of objects
std::vector<Object *> objects;

// Camera parameters
float cameraAngle = 0.0f;
float cameraDistance = 20.0f;
float cameraHeight = 5.0f;

void init()
{
    // Set background color to dark gray
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Enable lighting with more prominent shading
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // More noticeable lighting for better depth perception
    GLfloat lightPos[] = {5.0f, 5.0f, 5.0f, 1.0f};
    GLfloat lightAmbient[] = {0.3f, 0.3f, 0.3f, 1.0f};  // Lower ambient for more contrast
    GLfloat lightDiffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};  // Higher diffuse for clear shading
    GLfloat lightSpecular[] = {0.5f, 0.5f, 0.5f, 1.0f}; // Some specular for highlights

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

    // Enable color material
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // Set material properties for specular highlights
    GLfloat matSpecular[] = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat matShininess[] = {32.0f};
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);


    // Sun
    Sphere *sphere = new Sphere(1.2f, 30, 30);
    sphere->setPosition(0, 0, -10.0f);
    sphere->setColor(0.92f, 0.87f, 0.45f); 
    objects.push_back(sphere);

    //Sand
    Cube* cube = new Cube(1);
    cube->setPosition(0.0f, -3.0f, 0.0f);
    cube->setScale(100.0f, 0.0f, 20.0f);
    cube->setColor(0.88f, 0.75f, 0.57f);
    objects.push_back(cube);

}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

   

    // Position camera in a circular path
    float camX = cameraDistance * sin(cameraAngle * 3.14159f / 180.0f);
    float camZ = cameraDistance * cos(cameraAngle * 3.14159f / 180.0f);

    gluLookAt(camX, cameraHeight, camZ, // Eye position
              0.0, 0.0, 0.0,            // Look at point
              0.0, 1.0, 0.0);           // Up vector

    glBegin(GL_QUADS);
    // Define top-left color and vertex
    glColor3f(0.2f, 0.2f, 0.6f);
    glVertex2f(-1.0f, 1.0f);

    // Define top-right color and vertex (can be same as top-left for vertical gradient)
    glColor3f(0.2f, 0.2f, 0.6f);
    glVertex2f(1.0f, 1.0f);

    // Define bottom-right color and vertex
    glColor3f(0.9f, 0.9f, 1.0f);
    glVertex2f(1.0f, -1.0f);

    // Define bottom-left color and vertex (can be same as bottom-right for vertical gradient)
    glColor3f(0.9f, 0.9f, 1.0f);
    glVertex2f(-1.0f, -1.0f);
    glEnd();

    // Draw all objects
    for (size_t i = 0; i < objects.size(); i++)
    {
        objects[i]->render();
    }

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0, (double)w / (double)h, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
}

void update(int value)
{
    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // ~60 FPS
}

int main(int argc, char **argv)
{
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("3D Shapes Sample Code");

    // Initialize OpenGL settings
    init();

    // Register callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, update, 0);

    // Start main loop
    glutMainLoop();

    return 0;
}
