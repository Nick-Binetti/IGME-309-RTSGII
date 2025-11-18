#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

// Simple vertex shader - just passes through position and color
const char *vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vertexColor;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    vertexColor = aColor;
}
)";

// Simple fragment shader - outputs the interpolated color
const char *fragmentShaderSource = R"(
#version 330 core
in vec3 vertexColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(vertexColor, 1.0);
}
)";

// Global variables
GLuint shaderProgram;
GLuint VAO, VBO;

// Compile a shader from source code
GLuint compileShader(GLenum type, const char *source)
{
    // Create shader object
    GLuint shader = glCreateShader(type);

    // Attach source code and compile
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    // Check for errors
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation error: " << infoLog << std::endl;
    }

    return shader;
}

// Create and link shader program
GLuint createShaderProgram()
{
    // Compile both shaders
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    // Create program and link shaders
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // Check for linking errors
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Program linking error: " << infoLog << std::endl;
    }

    // Clean up shaders (no longer needed after linking)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

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

    // Configure vertex attribute: position (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // If location not explicitly defined:
    // GLint posLoc = glGetAttribLocation(program, "aPos");
    // glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    // glEnableVertexAttribArray(posLoc);

    // Configure vertex attribute: color (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind VAO
    glBindVertexArray(0);

    // Create shader program
    shaderProgram = createShaderProgram();

    std::cout << "OpenGL initialized successfully" << std::endl;
}

// Display callback - called every frame
void display()
{
    // Clear the screen
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Use our shader program
    glUseProgram(shaderProgram);

    // Draw the triangle
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
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
    glutCreateWindow("Basic OpenGL Shader Setup");

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
    std::cout << "Press ESC to exit" << std::endl;
    glutMainLoop();

    return 0;
}
