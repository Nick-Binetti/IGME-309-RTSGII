#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cmath>

// Shader sources - using built-in matrix uniforms
const char *vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vertexColor;

uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(aPos, 1.0);
    vertexColor = aColor;
}
)";

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
float rotationAngle = 0.0f;
int windowWidth = 800;
int windowHeight = 600;

// Animation timing variables
float animationTime = 0.0f;     // Current time in animation cycle (0.0 to 1.0)
float animationDuration = 2.0f; // Duration of one full rotation in seconds

GLuint compileShader(GLenum type, const char *source)
{
    // Create a shader object of the specified type (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)
    GLuint shader = glCreateShader(type);

    // Attach the source code to the shader object
    // Parameters: shader object, number of strings, array of strings, array of string lengths (NULL = null-terminated)
    glShaderSource(shader, 1, &source, nullptr);

    // Compile the shader source code into GPU instructions
    glCompileShader(shader);

    // Check if compilation was successful
    GLint success;
    // Get the compilation status (GL_TRUE or GL_FALSE)
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        // If compilation failed, retrieve the error message
        char infoLog[512];
        // Get the shader info log (error messages)
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        // Print the error to the console
        std::cerr << "Shader compilation error: " << infoLog << std::endl;
    }

    // Return the compiled shader object ID
    return shader;
}

GLuint createShaderProgram()
{
    // Compile both vertex and fragment shaders
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    // Create a program object to link shaders together
    GLuint program = glCreateProgram();

    // Attach the vertex shader to the program
    glAttachShader(program, vertexShader);

    // Attach the fragment shader to the program
    glAttachShader(program, fragmentShader);

    // Link the vertex and fragment shaders into a complete program
    // This connects shader inputs/outputs and prepares the program for use
    glLinkProgram(program);

    // Check if linking was successful
    GLint success;
    // Get the link status (GL_TRUE or GL_FALSE)
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success)
    {
        // If linking failed, retrieve the error message
        char infoLog[512];
        // Get the program info log (error messages)
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        // Print the error to the console
        std::cerr << "Shader program linking error: " << infoLog << std::endl;
    }

    // Delete the shader objects since they're now linked into the program
    // They're no longer needed individually
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Return the linked shader program ID
    return program;
}

void initGL()
{
    // Triangle vertices - larger size
    float vertices[] = {
        // positions        // colors
        0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,   // top - red
        -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left - green
        1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f   // bottom right - blue
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    shaderProgram = createShaderProgram();

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    std::cout << "OpenGL initialization complete" << std::endl;
}

// Build the model matrix with rotation
glm::mat4 buildModelMatrix(float angleInDegrees)
{
    // Convert angle from degrees to radians
    float angleInRadians = angleInDegrees * 3.14159265358979323846f / 180.0f;

    // Calculate cosine and sine for rotation
    float c = cos(angleInRadians);
    float s = sin(angleInRadians);

    // Create identity matrix
    glm::mat4 model(1.0f);

    // Apply Y-axis rotation manually
    // Rotation matrix for Y-axis:
    // [cos θ   0   sin θ   0]
    // [  0     1     0     0]
    // [-sin θ  0   cos θ   0]
    // [  0     0     0     1]
    model[0][0] = c;  // Top-left: cos(theta)
    model[0][2] = s;  // Column 0, row 2: sin(theta)
    model[2][0] = -s; // Column 2, row 0: -sin(theta)
    model[2][2] = c;  // Bottom-right: cos(theta)

    return model;
}

// Build the view matrix (camera position and orientation)
glm::mat4 buildViewMatrix()
{
    // Camera position
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.5f, 5.0f);

    // Point the camera is looking at (center of world)
    glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);

    // Up direction (Y-axis is up)
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    // Create view matrix using lookAt
    return glm::lookAt(cameraPos, target, up);
}

// Build the projection matrix (perspective)
glm::mat4 buildProjectionMatrix()
{
    // Calculate aspect ratio from global window dimensions
    float aspectRatio = (float)windowWidth / (float)windowHeight;

    // Field of view in degrees, then convert to radians
    float fov = 45.0f * 3.14159265f / 180.0f;

    // Calculate tangent of half the field of view
    float tanHalfFov = tan(fov / 2.0f);

    // Near and far clipping planes
    float zNear = 0.1f;
    float zFar = 100.0f;

    // Build perspective projection matrix manually
    glm::mat4 projection(0.0f);

    // Column 0: X scaling based on aspect ratio and FOV
    projection[0][0] = 1.0f / (aspectRatio * tanHalfFov);

    // Column 1: Y scaling based on FOV
    projection[1][1] = 1.0f / tanHalfFov;

    // Column 2: Z mapping (depth calculations)
    projection[2][2] = -(zFar + zNear) / (zFar - zNear);
    projection[2][3] = -1.0f; // Perspective divide trigger

    // Column 3: Z translation for depth
    projection[3][2] = -(2.0f * zFar * zNear) / (zFar - zNear);

    return projection;
}

// Cubic Bézier function
// Evaluates a cubic Bézier curve at parameter t with control points p0, p1, p2, p3
float cubicBezier(float t, float p0, float p1, float p2, float p3)
{
    float u = 1.0f - t;
    return u * u * u * p0 + 3 * u * u * t * p1 + 3 * u * t * t * p2 + t * t * t * p3;
}

// Solve for t given x using Newton-Raphson method
// This finds the t value that produces a given x coordinate on the Bézier curve
float solveBezierX(float x, float x1, float x2)
{
    float t = x;
    for (int i = 0; i < 8; i++)
    {
        float xt = cubicBezier(t, 0.0f, x1, x2, 1.0f);
        float dx = xt - x;
        if (fabs(dx) < 0.0001f)
            break;
        float u = 1.0f - t;
        float derivative = 3 * u * u * (x1 - 0.0f) + 6 * u * t * (x2 - x1) + 3 * t * t * (1.0f - x2);
        if (fabs(derivative) < 0.000001f)
            break;
        t -= dx / derivative;
        t = fmax(0.0f, fmin(1.0f, t));
    }
    return t;
}

// Cubic-bezier easing function
// Creates smooth animation curves similar to CSS cubic-bezier()
// Parameters (x1, y1, x2, y2) define the two control points of the Bézier curve
float cubicBezierEase(float x, float x1, float y1, float x2, float y2)
{
    if (x <= 0.0f)
        return 0.0f;
    if (x >= 1.0f)
        return 1.0f;
    float t = solveBezierX(x, x1, x2);
    return cubicBezier(t, 0.0f, y1, y2, 1.0f);
}

void display()
{
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);

    // Build transformation matrices using helper functions
    glm::mat4 model = buildModelMatrix(rotationAngle);
    glm::mat4 view = buildViewMatrix();
    glm::mat4 projection = buildProjectionMatrix();

    // Combine matrices: MVP = Projection * View * Model
    glm::mat4 mvp = projection * view * model;

    // Send combined MVP matrix to shader
    GLint mvpLoc = glGetUniformLocation(shaderProgram, "mvp");
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

    // Draw the triangle
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    glutSwapBuffers();
}

void timer(int value)
{
    // Update animation time (normalized 0.0 to 1.0)
    animationTime += 0.016f / animationDuration; // ~60 FPS (16ms per frame)

    // Reset animation when it completes one cycle
    if (animationTime >= 1.0f)
    {
        animationTime = 0.0f;
    }

    // Apply cubic-bezier easing (ease-in-out curve)
    // Control points: (0.42, 0, 0.58, 1) - starts slow, speeds up, then slows down
    float easedTime = cubicBezierEase(animationTime, 0.42f, 0.0f, 0.58f, 1.0f);

    // Map eased time to rotation angle (0 to 360 degrees)
    rotationAngle = easedTime * 360.0f;

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // ~60 FPS
}

void reshape(int width, int height)
{
    if (height == 0)
        height = 1;

    // Update global window dimensions
    windowWidth = width;
    windowHeight = height;

    // Update OpenGL viewport
    glViewport(0, 0, width, height);
}

void keyboard(unsigned char key, int x, int y)
{
    if (key == 27)
    {
        exit(0);
    }
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL Rotating Triangle");

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    initGL();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);

    std::cout << "Starting main loop..." << std::endl;
    glutMainLoop();

    return 0;
}
