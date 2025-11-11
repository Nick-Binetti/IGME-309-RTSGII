#include <GL/glut.h>
#include <cmath>
#include <algorithm>

#define M_PI 3.1415926f

// Triangle vertices (local coordinates)
float triangleVerts[3][2] = {
    {0.0f, 0.15f},
    {-0.12f, -0.09f},
    {0.12f, -0.09f}};

// Position and rotation
float posX = 0.0f;
float posY = 0.0f;
float rotation = 0.0f;
float velocityX = 0.15f;
float velocityY = 0.1f;
float rotationSpeed = 50.0f;

// Bounding volume toggle (false = AABB, true = Circle)
bool useBoundingCircle = false;

// Transformed vertices (world space)
float transformedVerts[3][2];

void transformVertices()
{
    float cosR = cos(rotation * M_PI / 180.0f);
    float sinR = sin(rotation * M_PI / 180.0f);

    for (int i = 0; i < 3; i++)
    {
        // Rotate then translate
        transformedVerts[i][0] = triangleVerts[i][0] * cosR - triangleVerts[i][1] * sinR + posX;
        transformedVerts[i][1] = triangleVerts[i][0] * sinR + triangleVerts[i][1] * cosR + posY;
    }
}

void drawAABB(float verts[3][2])
{
    float minX = verts[0][0];
    float maxX = verts[0][0];
    float minY = verts[0][1];
    float maxY = verts[0][1];

    for (int i = 1; i < 3; i++)
    {
        minX = std::min(minX, verts[i][0]);
        maxX = std::max(maxX, verts[i][0]);
        minY = std::min(minY, verts[i][1]);
        maxY = std::max(maxY, verts[i][1]);
    }

    glColor3f(1.0f, 0.0f, 0.0f); // Red
    glBegin(GL_LINE_LOOP);
    glVertex2f(minX, minY);
    glVertex2f(maxX, minY);
    glVertex2f(maxX, maxY);
    glVertex2f(minX, maxY);
    glEnd();
}

void drawBoundingCircle(float verts[3][2])
{
    // Compute center as average of vertices
    float centerX = 0.0f;
    float centerY = 0.0f;
    for (int i = 0; i < 3; i++)
    {
        centerX += verts[i][0];
        centerY += verts[i][1];
    }
    centerX /= 3.0f;
    centerY /= 3.0f;

    // Find maximum distance from center
    float radius = 0.0f;
    for (int i = 0; i < 3; i++)
    {
        float dx = verts[i][0] - centerX;
        float dy = verts[i][1] - centerY;
        float dist = sqrt(dx * dx + dy * dy);
        radius = std::max(radius, dist);
    }

    glColor3f(1.0f, 0.0f, 0.0f); // Red
    glBegin(GL_LINE_LOOP);
    int segments = 50;
    for (int i = 0; i < segments; i++)
    {
        float angle = 2.0f * M_PI * i / segments;
        float x = centerX + radius * cos(angle);
        float y = centerY + radius * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Transform vertices
    transformVertices();

    // Draw triangle (green)
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < 3; i++)
    {
        glVertex2f(transformedVerts[i][0], transformedVerts[i][1]);
    }
    glEnd();

    // Draw bounding volume (red)
    if (useBoundingCircle)
    {
        drawBoundingCircle(transformedVerts);
    }
    else
    {
        drawAABB(transformedVerts);
    }

    glutSwapBuffers();
}

void update(int value)
{
    // Update position
    posX += velocityX * 0.016f; // ~60 FPS
    posY += velocityY * 0.016f;

    // Bounce off walls
    if (posX > 0.6f || posX < -0.6f)
        velocityX = -velocityX;
    if (posY > 0.6f || posY < -0.6f)
        velocityY = -velocityY;

    // Update rotation
    rotation += rotationSpeed * 0.016f;
    if (rotation > 360.0f)
        rotation -= 360.0f;

    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // ~60 FPS
}

void keyboard(unsigned char key, int x, int y)
{
    if (key == ' ')
    {
        useBoundingCircle = !useBoundingCircle;
        glutPostRedisplay();
    }
    else if (key == 27)
    { // ESC
        exit(0);
    }
}

void init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("2D Bounding Volumes");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, update, 0);

    glutMainLoop();
    return 0;
}