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
// PRACTICE: Complete this function to create a Y-axis rotation matrix
glm::mat4 buildModelMatrix(float angleInDegrees)
{
    // TODO: Convert angle from degrees to radians
    // Hint: radians = degrees * PI / 180
    float angleInRadians = 0.0f; // REPLACE THIS

    // TODO: Calculate cosine and sine for rotation
    // Hint: Use cos() and sin() functions
    float c = 0.0f; // REPLACE THIS
    float s = 0.0f; // REPLACE THIS

    // TODO: Create identity matrix
    glm::mat4 model(1.0f);

    // TODO: Apply Y-axis rotation to the matrix
    // Y-axis rotation matrix formula:
    // [cos θ   0   sin θ   0]
    // [  0     1     0     0]
    // [-sin θ  0   cos θ   0]
    // [  0     0     0     1]
    // Hint: model[column][row] - remember GLM uses column-major ordering
    // WRITE YOUR CODE HERE

    return model;
}

// Build the view matrix (camera position and orientation)
// PRACTICE: Complete this function to position the camera
glm::mat4 buildViewMatrix()
{
    // TODO: Define camera position in world space
    // Hint: Position it at (0, 0.5, 5) to see the triangle from slightly above
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f); // REPLACE THIS

    // TODO: Define the point the camera is looking at
    // Hint: Look at the origin (0, 0, 0) - center of the world
    glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f); // REPLACE THIS

    // TODO: Define the up direction
    // Hint: Y-axis is typically "up" in 3D graphics
    glm::vec3 up = glm::vec3(0.0f, 0.0f, 0.0f); // REPLACE THIS

    // TODO: Create view matrix using lookAt
    // Hint: glm::lookAt(eye, center, up)
    return glm::mat4(1.0f); // REPLACE THIS
}

// Build the projection matrix (perspective)
// PRACTICE: Complete this function to create a perspective projection
// The projection matrix transforms 3D camera space coordinates into 2D screen space
// It creates the illusion of depth - objects farther away appear smaller
glm::mat4 buildProjectionMatrix()
{
    // TODO: Calculate aspect ratio from window dimensions
    // Aspect ratio prevents distortion when window width != height
    // Hint: aspect = width / height
    float aspectRatio = 1.0f; // REPLACE THIS

    // TODO: Define field of view (FOV) and convert to radians
    // FOV controls how "wide" the camera sees (like zoom on a camera)
    // Hint: 45 degrees is a common FOV, PI = 3.14159265f
    float fov = 0.0f; // REPLACE THIS

    // TODO: Calculate tangent of half the field of view
    // This is used in the projection matrix formula
    // Hint: Use tan() function
    float tanHalfFov = 0.0f; // REPLACE THIS

    // TODO: Define near and far clipping planes
    // Near plane: closest distance camera can see
    // Far plane: farthest distance camera can see
    // Objects outside [near, far] range are not rendered
    // Hint: Near = 0.1, Far = 100.0 are common values
    float zNear = 0.0f; // REPLACE THIS
    float zFar = 0.0f;  // REPLACE THIS

    // TODO: Build perspective projection matrix
    // Perspective projection matrix formula (OpenGL convention):
    // [1/(aspect*tan(fov/2))      0                    0                           0              ]
    // [        0            1/tan(fov/2)               0                           0              ]
    // [        0                  0          -(far+near)/(far-near)    -(2*far*near)/(far-near)  ]
    // [        0                  0                   -1                           0              ]
    //
    // Remember: GLM uses column-major ordering, so matrix[column][row]
    glm::mat4 projection(0.0f);

    // TODO: Set element [0][0] - X scaling (accounts for aspect ratio and FOV)
    // This controls horizontal field of view
    // Formula: 1.0 / (aspectRatio * tanHalfFov)
    // WRITE YOUR CODE HERE

    // TODO: Set element [1][1] - Y scaling (accounts for FOV)
    // This controls vertical field of view
    // Formula: 1.0 / tanHalfFov
    // WRITE YOUR CODE HERE

    // TODO: Set element [2][2] - Z mapping (depth calculations)
    // Maps depth range [near, far] to normalized device coordinates [-1, 1]
    // Formula: -(zFar + zNear) / (zFar - zNear)
    // WRITE YOUR CODE HERE

    // TODO: Set element [2][3] - Perspective divide trigger
    // This value (-1) causes the w-component to be set to -z
    // The GPU then divides x,y,z by w to create perspective effect
    // Value: -1.0
    // WRITE YOUR CODE HERE

    // TODO: Set element [3][2] - Z translation component
    // Adjusts depth values for proper depth testing
    // Formula: -(2.0 * zFar * zNear) / (zFar - zNear)
    // WRITE YOUR CODE HERE

    return projection;
}

void display()
{
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);

    // === MODEL-VIEW-PROJECTION (MVP) TRANSFORMATION PIPELINE ===
    //
    // To render 3D objects on a 2D screen, we need three transformations:
    //
    // 1. MODEL MATRIX: Positions/rotates/scales the object in the 3D world
    //    - Transforms from "object space" to "world space"
    //    - Example: Rotating our triangle
    //
    // 2. VIEW MATRIX: Positions and orients the camera in the world
    //    - Transforms from "world space" to "camera space"
    //    - Example: Moving camera back to see the triangle
    //
    // 3. PROJECTION MATRIX: Creates perspective (objects far away look smaller)
    //    - Transforms from "camera space" to "clip space" (normalized coordinates)
    //    - Example: Making distant objects appear smaller
    //
    // These are combined: MVP = Projection * View * Model
    // Applied right-to-left: vertex -> Model -> View -> Projection -> screen

    // Build transformation matrices using helper functions
    glm::mat4 model = buildModelMatrix(rotationAngle);
    glm::mat4 view = buildViewMatrix();
    glm::mat4 projection = buildProjectionMatrix();

    // Combine matrices: MVP = Projection * View * Model
    // Order matters! Matrix multiplication is NOT commutative
    // We multiply right-to-left: projection * view * model
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
    rotationAngle += 0.5f; // Slower rotation (was 1.0f)
    if (rotationAngle > 360.0f)
    {
        rotationAngle -= 360.0f;
    }

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
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
