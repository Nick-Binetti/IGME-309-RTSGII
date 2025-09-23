#include "Triangles.h"

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutCreateWindow("Multiple Triangles Example");

    App myApp;
    app = &myApp;

    // Add a few triangles
    app->triangles.emplace_back(-0.5f, 0.0f, 1.0f, 0.0f, 0.0f); // red left
    app->triangles.emplace_back(0.5f, 0.0f, 0.0f, 1.0f, 0.0f);  // green right
    app->triangles.emplace_back(0.0f, 0.5f, 0.0f, 0.0f, 1.0f);  // blue top

    // Hook GLUT to app->render
    glutDisplayFunc(+[]
                    {
                        app->render();
                    });

    glutMainLoop();
    return 0;
}
