#ifdef __APPLE__
#include <GLUT/glut.h> // include glut for Mac
#else
#include <GL/freeglut.h> //include glut for Windows
#endif

#include <iostream>

// Mouse button press/release
void mouse(int button, int state, int x, int y)
{
    const char *btn = (button == GLUT_LEFT_BUTTON) ? "Left" : (button == GLUT_RIGHT_BUTTON) ? "Right"
                                                          : (button == GLUT_MIDDLE_BUTTON)  ? "Middle"
                                                                                            : "Other";

    if (state == GLUT_DOWN)
        std::cout << btn << " button pressed at (" << x << "," << y << ")\n";
    else if (state == GLUT_UP)
        std::cout << btn << " button released at (" << x << "," << y << ")\n";
}

// Mouse motion with button pressed
void motion(int x, int y)
{
    std::cout << "Mouse dragging at (" << x << "," << y << ")\n";
}

// Mouse motion with no buttons pressed
void passiveMotion(int x, int y)
{
    std::cout << "Mouse moving at (" << x << "," << y << ")\n";
}

// Mouse wheel (FreeGLUT extension)
void mouseWheel(int wheel, int direction, int x, int y)
{
    std::cout << "Mouse wheel: " << (direction > 0 ? "up" : "down")
              << " at (" << x << "," << y << ")\n";
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(400, 300);
    glutCreateWindow("Mouse Callback Demo");

    glutDisplayFunc(display);

    // Register mouse callbacks
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutPassiveMotionFunc(passiveMotion);
    glutMouseWheelFunc(mouseWheel); // only in FreeGLUT

    glutMainLoop();
    return 0;
}
