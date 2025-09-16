#ifdef __APPLE__
#include <GLUT/glut.h> // include glut for Mac
#else
#include <GL/freeglut.h> //include glut for Windows
#endif

#include <iostream>

// Normal key press
void keyboardDown(unsigned char key, int x, int y)
{
    std::cout << "Normal key pressed: " << key << " at (" << x << "," << y << ")\n";
}

// Normal key release
void keyboardUp(unsigned char key, int x, int y)
{
    std::cout << "Normal key released: " << key << " at (" << x << "," << y << ")\n";
}

// Special key press (arrows, F1–F12, etc.)
void specialDown(int key, int x, int y)
{
    std::cout << "Special key pressed: " << key << " at (" << x << "," << y << ")\n";
}

// Special key release
void specialUp(int key, int x, int y)
{
    std::cout << "Special key released: " << key << " at (" << x << "," << y << ")\n";
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
    glutCreateWindow("Keyboard Callback Demo");

    glutDisplayFunc(display);

    // Register all four keyboard callbacks
    glutKeyboardFunc(keyboardDown);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(specialDown);
    glutSpecialUpFunc(specialUp);

    glutMainLoop();
    return 0;
}
