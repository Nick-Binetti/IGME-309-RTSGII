#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <thread>

#define M_PI 3.1415926f

// Window dimensions
const int WIDTH = 600;
const int HEIGHT = 600;

// Polygon properties
int vertNum = 6;
float ballCenter[2] = {0.0f, 0.0f};
float radius = 0.5f;

// Triangle properties
float triangleRotation = 0.0f; // Self-rotation angle
float triangleDistance = 0.6f; // Distance from center (fixed position)
float triangleSize = 0.15f;

// Threshold for edge detection
float edgeThreshold = 0.02f;

// State
bool paused = false;
bool useBoundingCircle = false;
int frameCount = 0;
float fps = 0.0f;
int lastTime = 0;
float detectionTime = 0.0f; // Time in microseconds for collision detection

// Triangle color state
enum TriangleState
{
    OUTSIDE,
    ON_EDGE,
    INSIDE
};
TriangleState currentState = OUTSIDE;

// Get triangle vertices using GLM rotation matrix
void getTriangleVertices(float vertices[3][2])
{
    // Triangle center is fixed above the polygon
    glm::vec2 center(ballCenter[0], ballCenter[1] + triangleDistance);

    // Create rotation matrix
    glm::mat2 rotation(
        cos(triangleRotation), -sin(triangleRotation),
        sin(triangleRotation), cos(triangleRotation));

    // Define base triangle vertices (equilateral triangle pointing up)
    glm::vec2 baseVertices[3] = {
        glm::vec2(0.0f, triangleSize),                           // Top vertex
        glm::vec2(-triangleSize * 0.866f, -triangleSize * 0.5f), // Bottom left
        glm::vec2(triangleSize * 0.866f, -triangleSize * 0.5f)   // Bottom right
    };

    // Apply rotation and translation
    for (int i = 0; i < 3; ++i)
    {
        glm::vec2 v = rotation * baseVertices[i] + center;
        vertices[i][0] = v.x;
        vertices[i][1] = v.y;
    }

    //
    // Alternatively, you can always calculate the vertices manually.
    //
    // // Triangle center is fixed above the polygon
    // float centerX = ballCenter[0];
    // float centerY = ballCenter[1] + triangleDistance;
    // // Equilateral triangle with its own rotation
    // for (int i = 0; i < 3; ++i)
    // {
    //     float angle = triangleRotation + (i * 2.0f * M_PI / 3.0f);
    //     vertices[i][0] = centerX + triangleSize * cos(angle);
    //     vertices[i][1] = centerY + triangleSize * sin(angle);
    // }
}

// Ray casting algorithm for point-in-polygon test
bool pointInPolygon(float x, float y, const std::vector<float> &polyX, const std::vector<float> &polyY)
{
    int n = polyX.size();
    bool inside = false;

    for (int i = 0, j = n - 1; i < n; j = i++)
    {
        if (((polyY[i] > y) != (polyY[j] > y)) &&
            (x < (polyX[j] - polyX[i]) * (y - polyY[i]) / (polyY[j] - polyY[i]) + polyX[i]))
        {
            inside = !inside;
        }
    }
    return inside;
}

// Check if point is on edge of polygon (within threshold)
bool pointOnEdge(float x, float y, const std::vector<float> &polyX, const std::vector<float> &polyY, float threshold = 0.02f)
{
    int n = polyX.size();

    for (int i = 0; i < n; ++i)
    {
        int j = (i + 1) % n;
        float x1 = polyX[i], y1 = polyY[i];
        float x2 = polyX[j], y2 = polyY[j];

        // Distance from point to line segment
        float dx = x2 - x1;
        float dy = y2 - y1;
        float lengthSq = dx * dx + dy * dy;

        if (lengthSq < 0.0001f)
            continue;

        float t = ((x - x1) * dx + (y - y1) * dy) / lengthSq;
        t = fmax(0.0f, fmin(1.0f, t));

        float closestX = x1 + t * dx;
        float closestY = y1 + t * dy;

        float distSq = (x - closestX) * (x - closestX) + (y - closestY) * (y - closestY);

        if (distSq < threshold * threshold)
        {
            return true;
        }
    }
    return false;
}

// Bounding circle collision detection
bool pointInCircle(float x, float y, float cx, float cy, float r)
{
    float dx = x - cx;
    float dy = y - cy;
    return (dx * dx + dy * dy) <= (r * r);
}

bool pointOnCircleEdge(float x, float y, float cx, float cy, float r, float threshold = 0.02f)
{
    float dx = x - cx;
    float dy = y - cy;
    float dist = sqrt(dx * dx + dy * dy);
    return fabs(dist - r) < threshold;
}

// Check triangle state
TriangleState checkTriangleState()
{
    auto start = std::chrono::high_resolution_clock::now();

    float vertices[3][2];
    getTriangleVertices(vertices);

    TriangleState result;

    if (useBoundingCircle)
    {
        // Bounding circle detection
        int onEdgeCount = 0;
        int insideCount = 0;

        for (int i = 0; i < 3; ++i)
        {
            if (pointOnCircleEdge(vertices[i][0], vertices[i][1], ballCenter[0], ballCenter[1], radius, edgeThreshold))
            {
                onEdgeCount++;
            }
            else if (pointInCircle(vertices[i][0], vertices[i][1], ballCenter[0], ballCenter[1], radius))
            {
                insideCount++;
            }
        }

        if (onEdgeCount > 0)
            result = ON_EDGE;
        else if (insideCount > 0)
            result = INSIDE;
        else
            result = OUTSIDE;
    }
    else
    {
        // Ray casting detection
        std::vector<float> polyX, polyY;
        for (int i = 0; i < vertNum; ++i)
        {
            float t = (float)i / vertNum * 2.0f * M_PI;
            polyX.push_back(ballCenter[0] + radius * cos(t));
            polyY.push_back(ballCenter[1] + radius * sin(t));

            // Add tiny sleep per vertex to simulate more expensive computation
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }

        int onEdgeCount = 0;
        int insideCount = 0;

        for (int i = 0; i < 3; ++i)
        {
            if (pointOnEdge(vertices[i][0], vertices[i][1], polyX, polyY, edgeThreshold))
            {
                onEdgeCount++;
            }
            else if (pointInPolygon(vertices[i][0], vertices[i][1], polyX, polyY))
            {
                insideCount++;
            }
        }

        if (onEdgeCount > 0)
            result = ON_EDGE;
        else if (insideCount > 0)
            result = INSIDE;
        else
            result = OUTSIDE;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    detectionTime = duration.count();

    return result;
}

void drawCircle()
{
    glColor3f(0.7f, 0.7f, 0.75f);
    // draw the circle
    glBegin(GL_POLYGON);
    for (int i = 0; i < vertNum; ++i)
    {
        float t = (float)i / vertNum * 2.0f * M_PI;
        glVertex2f(ballCenter[0] + radius * cos(t), ballCenter[1] + radius * sin(t));
    }
    glEnd();
}

// Draw threshold visualization
void drawThresholdVisualization()
{
    if (useBoundingCircle)
    {
        // Draw bounding circle
        glColor3f(0.0f, 1.0f, 1.0f); // Cyan for bounding circle
        glBegin(GL_LINE_LOOP);
        int numSegments = 100;
        for (int i = 0; i < numSegments; ++i)
        {
            float t = (float)i / numSegments * 2.0f * M_PI;
            glVertex2f(ballCenter[0] + radius * cos(t), ballCenter[1] + radius * sin(t));
        }
        glEnd();

        // Draw inner threshold circle (red)
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < numSegments; ++i)
        {
            float t = (float)i / numSegments * 2.0f * M_PI;
            glVertex2f(ballCenter[0] + (radius - edgeThreshold) * cos(t),
                       ballCenter[1] + (radius - edgeThreshold) * sin(t));
        }
        glEnd();

        // Draw outer threshold circle (green)
        glColor3f(0.0f, 1.0f, 0.0f);
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < numSegments; ++i)
        {
            float t = (float)i / numSegments * 2.0f * M_PI;
            glVertex2f(ballCenter[0] + (radius + edgeThreshold) * cos(t),
                       ballCenter[1] + (radius + edgeThreshold) * sin(t));
        }
        glEnd();
    }
    else
    {
        // Draw polygon edges with threshold zones
        std::vector<float> polyX, polyY;
        for (int i = 0; i < vertNum; ++i)
        {
            float t = (float)i / vertNum * 2.0f * M_PI;
            polyX.push_back(ballCenter[0] + radius * cos(t));
            polyY.push_back(ballCenter[1] + radius * sin(t));
        }

        // Draw inner threshold (red - inside polygon)
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < vertNum; ++i)
        {
            // Offset vertex radially inward
            float dx = polyX[i] - ballCenter[0];
            float dy = polyY[i] - ballCenter[1];
            float dist = sqrt(dx * dx + dy * dy);
            if (dist > 0.0001f)
            {
                float scale = (dist - edgeThreshold) / dist;
                glVertex2f(ballCenter[0] + dx * scale, ballCenter[1] + dy * scale);
            }
        }
        glEnd();

        // Draw outer threshold (green - outside polygon)
        glColor3f(0.0f, 1.0f, 0.0f);
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < vertNum; ++i)
        {
            // Offset vertex radially outward
            float dx = polyX[i] - ballCenter[0];
            float dy = polyY[i] - ballCenter[1];
            float dist = sqrt(dx * dx + dy * dy);
            if (dist > 0.0001f)
            {
                float scale = (dist + edgeThreshold) / dist;
                glVertex2f(ballCenter[0] + dx * scale, ballCenter[1] + dy * scale);
            }
        }
        glEnd();
    }
}

void drawTriangle()
{
    float vertices[3][2];
    getTriangleVertices(vertices);

    // Set color based on state
    if (currentState == INSIDE)
    {
        glColor3f(1.0f, 0.0f, 0.0f); // Red
    }
    else if (currentState == ON_EDGE)
    {
        glColor3f(1.0f, 1.0f, 0.0f); // Yellow
    }
    else
    {
        glColor3f(0.0f, 1.0f, 0.0f); // Green
    }

    glBegin(GL_TRIANGLES);
    for (int i = 0; i < 3; ++i)
    {
        glVertex2f(vertices[i][0], vertices[i][1]);
    }
    glEnd();
}

void drawText(float x, float y, const std::string &text)
{
    glRasterPos2f(x, y);
    for (char c : text)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }
}

void drawUI()
{
    glColor3f(1.0f, 1.0f, 1.0f);

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1);
    oss << "FPS: " << fps;
    drawText(-0.95f, 0.9f, oss.str());

    drawText(-0.95f, 0.85f, "Controls:");
    drawText(-0.95f, 0.80f, "A: Add vertex");
    drawText(-0.95f, 0.75f, "D: Remove vertex");
    drawText(-0.95f, 0.70f, "SPACE: Pause/Resume");

    std::string detectionMode = useBoundingCircle ? "X: Circle Mode (ON)" : "X: Polygon Mode";
    drawText(-0.95f, 0.65f, detectionMode);

    std::ostringstream vertStr;
    vertStr << "Vertices: " << vertNum;
    drawText(-0.95f, 0.60f, vertStr.str());

    std::string stateStr = paused ? "State: PAUSED" : "State: RUNNING";
    drawText(-0.95f, 0.55f, stateStr);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw polygon first
    drawCircle();

    // Draw threshold visualization on top of polygon
    drawThresholdVisualization();

    // Draw triangle
    drawTriangle();

    // Draw UI
    drawUI();

    glutSwapBuffers();

    // FPS calculation
    frameCount++;
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    if (currentTime - lastTime > 1000)
    {
        fps = frameCount * 1000.0f / (currentTime - lastTime);
        frameCount = 0;
        lastTime = currentTime;
    }
}

void update(int value)
{
    if (!paused)
    {
        // Rotate the triangle on its own axis
        triangleRotation += 0.01f;
        if (triangleRotation > 2.0f * M_PI)
        {
            triangleRotation -= 2.0f * M_PI;
        }

        currentState = checkTriangleState();
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // ~60 FPS
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'a':
    case 'A':
        if (vertNum < 1000)
            vertNum++;
        break;
    case 'd':
    case 'D':
        if (vertNum > 3)
            vertNum--;
        break;
    case ' ':
        paused = !paused;
        break;
    case 'x':
    case 'X':
        useBoundingCircle = !useBoundingCircle;
        break;
    case 27: // ESC
        exit(0);
        break;
    }
    glutPostRedisplay();
}

void init()
{
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Polygon-Triangle Collision Visualizer");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, update, 0);

    lastTime = glutGet(GLUT_ELAPSED_TIME);

    glutMainLoop();
    return 0;
}