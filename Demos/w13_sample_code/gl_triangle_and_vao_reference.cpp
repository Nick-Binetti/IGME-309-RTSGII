#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

// Global variables
GLuint VAO, VBO;

// Initialize OpenGL resources
void initGL()
{
    // Define a simple triangle with positions and colors
    float vertices[] = {
        // positions        // colors
        0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,   // top - red
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left - green
        0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f   // bottom right - blue
    };

    // Generate Vertex Array Object and Vertex Buffer Object
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind VAO first
    glBindVertexArray(VAO);

    // Bind and fill VBO with vertex data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Enable vertex attribute arrays for fixed-function pipeline
    // Position attribute (uses GL_VERTEX_ARRAY for fixed-function)
    glVertexPointer(3, GL_FLOAT, 6 * sizeof(float), (void *)0);
    glEnableClientState(GL_VERTEX_ARRAY);

    // Color attribute (uses GL_COLOR_ARRAY for fixed-function)
    glColorPointer(3, GL_FLOAT, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableClientState(GL_COLOR_ARRAY);

    // Unbind VAO
    glBindVertexArray(0);

    std::cout << "OpenGL initialized successfully" << std::endl;
}

// Display callback - called every frame
void display()
{
    // Clear the screen
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Make sure no shader is active (use fixed-function pipeline)
    glUseProgram(0);

    // Bind VAO containing our vertex data
    glBindVertexArray(VAO);

    // Draw the triangle using data from VAO
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Unbind VAO
    glBindVertexArray(0);

    // Swap buffers
    glutSwapBuffers();
}

// Keyboard callback
void keyboard(unsigned char key, int x, int y)
{
    if (key == 27)
    { // ESC key
        exit(0);
    }
}

int main(int argc, char **argv)
{
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Triangle with VAO");

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    // Initialize our OpenGL resources
    initGL();

    // Register callbacks
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    // Start main loop
    glutMainLoop();

    return 0;
}
