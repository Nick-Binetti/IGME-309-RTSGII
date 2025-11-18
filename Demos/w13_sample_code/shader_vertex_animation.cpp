#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glu.h>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cstdio>

// Vertex shader - animates particles using physics simulation
const char *vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aInitialVelocity;  // Initial velocity for this particle
layout (location = 1) in float aStartTime;       // When this particle was born
layout (location = 2) in vec3 aColor;            // Particle color

out vec3 particleColor;
out float particleAge;

uniform float uTime;              // Current simulation time
uniform vec3 uAcceleration;       // Gravity/acceleration
uniform float uMaxLifeTime;       // Maximum lifetime before reset

void main()
{
    // Calculate how long this particle has been alive
    float age = mod(uTime - aStartTime, uMaxLifeTime);

    // Physics simulation in vertex shader:
    // position = initialVelocity * time + 0.5 * acceleration * time^2
    vec3 velocity = aInitialVelocity + uAcceleration * age;
    vec3 position = aInitialVelocity * age + 0.5 * uAcceleration * age * age;

    // Convert to clip space coordinates (scale down for visibility)
    gl_Position = vec4(position * 0.1, 1.0);
    gl_PointSize = 3.0;

    // Pass color and age to fragment shader
    particleColor = aColor;
    particleAge = age;
}
)";

// Fragment shader - calculates alpha based on particle age
const char *fragmentShaderSource = R"(
#version 330 core
in vec3 particleColor;
in float particleAge;

out vec4 FragColor;

uniform float uMaxLifeTime;

void main()
{
    // Calculate alpha fallout in fragment shader
    // Fade out as particle ages (alpha decreases over lifetime)
    float alpha = 1.0 - (particleAge / uMaxLifeTime);

    FragColor = vec4(particleColor, alpha);
}
)";

// Global variables
GLuint shaderProgram;
GLuint VAO, VBO;
float simulationTime = 0.0f;
const int NUM_PARTICLES = 5000;
const float MAX_LIFETIME = 3.0f;

// FPS tracking variables
int frameCount = 0;
float fps = 0.0f;
float lastTime = 0.0f;
float lastFrameTime = 0.0f; // for simulation delta time

// Particle settings (similar to the original code)
const float minSpeedX = -2.0f, maxSpeedX = 2.0f;
const float minSpeedY = 5.0f, maxSpeedY = 10.0f;
const float minSpeedZ = -2.0f, maxSpeedZ = 2.0f;
const float acceleration[3] = {0.0f, -9.8f, 0.0f}; // Gravity

// Random value generator
float getRandomValue(float min_value, float max_value)
{
    return min_value + (std::rand()) / (RAND_MAX / (max_value - min_value));
}

// Compile shader
GLuint compileShader(GLenum type, const char *source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

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

// Create shader program
GLuint createShaderProgram()
{
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Program linking error: " << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

// Initialize particle system
void initGL()
{
    std::srand(std::time(nullptr));

    // Particle data: initial velocity (3 floats), start time (1 float), color (3 floats)
    // Total: 7 floats per particle
    float *particleData = new float[NUM_PARTICLES * 7];

    for (int i = 0; i < NUM_PARTICLES; i++)
    {
        int offset = i * 7;

        // Initial velocity (random within bounds)
        particleData[offset + 0] = getRandomValue(minSpeedX, maxSpeedX);
        particleData[offset + 1] = getRandomValue(minSpeedY, maxSpeedY);
        particleData[offset + 2] = getRandomValue(minSpeedZ, maxSpeedZ);

        // Start time (stagger particles so they don't all start at once)
        particleData[offset + 3] = -MAX_LIFETIME * i / NUM_PARTICLES;

        // Color (blue)
        particleData[offset + 4] = 0.0f; // R
        particleData[offset + 5] = 0.0f; // G
        particleData[offset + 6] = 1.0f; // B
    }

    // Create VAO and VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, NUM_PARTICLES * 7 * sizeof(float), particleData, GL_STATIC_DRAW);

    // Attribute 0: Initial velocity (3 floats)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Attribute 1: Start time (1 float)
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Attribute 2: Color (3 floats)
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)(4 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    delete[] particleData;

    shaderProgram = createShaderProgram();

    // Enable blending for alpha transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Enable point sprites
    glEnable(GL_PROGRAM_POINT_SIZE);

    std::cout << "Particle system initialized with " << NUM_PARTICLES << " particles" << std::endl;
}

// Draw text on screen using GLUT bitmap fonts
void drawText(const char *text, float x, float y)
{
    // Save current matrices
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 800);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Disable shader for text rendering
    glUseProgram(0);

    // Set text color to black
    glColor3f(0.0f, 0.0f, 0.0f);

    // Position text
    glRasterPos2f(x, y);

    // Draw each character
    for (const char *c = text; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);
    }

    // Restore matrices
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// Calculate and update FPS
void updateFPS()
{
    frameCount++;
    float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

    // Update FPS every second
    if (currentTime - lastTime >= 1.0f)
    {
        fps = frameCount / (currentTime - lastTime);
        frameCount = 0;
        lastTime = currentTime;
    }
}

// Display callback
void display()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);

    // Send uniforms to shader
    GLint timeLocation = glGetUniformLocation(shaderProgram, "uTime");
    GLint accelLocation = glGetUniformLocation(shaderProgram, "uAcceleration");
    GLint maxLifeLocation = glGetUniformLocation(shaderProgram, "uMaxLifeTime");

    glUniform1f(timeLocation, simulationTime);
    glUniform3fv(accelLocation, 1, acceleration);
    glUniform1f(maxLifeLocation, MAX_LIFETIME);

    // Draw all particles
    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, NUM_PARTICLES);
    glBindVertexArray(0);

    // Update and display FPS
    updateFPS();
    char fpsText[64];
    sprintf_s(fpsText, "FPS: %.1f", fps);
    drawText(fpsText, 10, 780);

    glutSwapBuffers();
}

// Idle callback for "unlimited" FPS
void idle()
{
    float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f; // seconds
    float dt = currentTime - lastFrameTime;
    if (dt < 0.0f)
        dt = 0.0f; // safety

    simulationTime += dt;
    lastFrameTime = currentTime;

    glutPostRedisplay();
}

// Keyboard callback
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
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 800);
    glutCreateWindow("GPU Particle System - Vertex Shader Physics");

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    initGL();

    // Initialize timing
    float t = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    lastTime = t;
    lastFrameTime = t;

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    // Use idle callback instead of timer for max FPS
    glutIdleFunc(idle);

    std::cout << "GPU particle fountain running. Press ESC to exit." << std::endl;

    glutMainLoop();

    return 0;
}
