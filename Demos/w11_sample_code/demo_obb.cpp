#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>

#define M_PI 3.1415926f

// Window dimensions
const int WIDTH = 600;
const int HEIGHT = 600;

// Rotation angle
float angle = 0.0f;

// Triangle vertices (in local space)
std::vector<glm::vec2> triangleVertices = {
    glm::vec2(0.0f, 0.3f),
    glm::vec2(-0.25f, -0.15f),
    glm::vec2(0.25f, -0.15f)};

struct OBB
{
    glm::vec2 center;
    glm::vec2 axes[2];     // Local x and y axes
    glm::vec2 halfExtents; // Half-widths along each axis
};

// Project vertices onto an axis
void projectVertices(const std::vector<glm::vec2> &vertices, const glm::vec2 &axis,
                     float &min, float &max)
{
    min = std::numeric_limits<float>::max();
    max = std::numeric_limits<float>::lowest();

    for (const auto &vertex : vertices)
    {
        float projection = glm::dot(vertex, axis);
        min = std::min(min, projection);
        max = std::max(max, projection);
    }
}

// Compute OBB for a set of vertices using covariance method
OBB computeOBB(const std::vector<glm::vec2> &vertices)
{
    OBB obb;

    // Compute centroid
    glm::vec2 centroid(0.0f);
    for (const auto &v : vertices)
    {
        centroid += v;
    }
    centroid /= static_cast<float>(vertices.size());

    // Compute covariance matrix
    float cxx = 0.0f, cxy = 0.0f, cyy = 0.0f;
    for (const auto &v : vertices)
    {
        glm::vec2 p = v - centroid;
        cxx += p.x * p.x;
        cxy += p.x * p.y;
        cyy += p.y * p.y;
    }
    cxx /= vertices.size();
    cxy /= vertices.size();
    cyy /= vertices.size();

    // Find eigenvectors (principal axes)
    // For 2D, we can use a simplified approach
    float trace = cxx + cyy;
    float det = cxx * cyy - cxy * cxy;
    float eigenvalue1 = trace / 2.0f + std::sqrt(std::max(0.0f, trace * trace / 4.0f - det));

    // First eigenvector
    if (std::abs(cxy) > 1e-6f)
    {
        obb.axes[0] = glm::normalize(glm::vec2(eigenvalue1 - cyy, cxy));
    }
    else
    {
        obb.axes[0] = glm::vec2(1.0f, 0.0f);
    }

    // Second eigenvector (perpendicular to first)
    obb.axes[1] = glm::vec2(-obb.axes[0].y, obb.axes[0].x);

    // Compute half-extents by projecting vertices onto axes
    float min0, max0, min1, max1;
    projectVertices(vertices, obb.axes[0], min0, max0);
    projectVertices(vertices, obb.axes[1], min1, max1);

    obb.halfExtents.x = (max0 - min0) / 2.0f;
    obb.halfExtents.y = (max1 - min1) / 2.0f;

    // Recompute center in world space based on projections
    glm::vec2 centerOnAxis0 = obb.axes[0] * ((min0 + max0) / 2.0f);
    glm::vec2 centerOnAxis1 = obb.axes[1] * ((min1 + max1) / 2.0f);
    obb.center = centerOnAxis0 + centerOnAxis1;

    return obb;
}

// Get the 4 corners of the OBB
std::vector<glm::vec2> getOBBCorners(const OBB &obb)
{
    std::vector<glm::vec2> corners(4);

    glm::vec2 ext0 = obb.axes[0] * obb.halfExtents.x;
    glm::vec2 ext1 = obb.axes[1] * obb.halfExtents.y;

    corners[0] = obb.center - ext0 - ext1;
    corners[1] = obb.center + ext0 - ext1;
    corners[2] = obb.center + ext0 + ext1;
    corners[3] = obb.center - ext0 + ext1;

    return corners;
}

// Transform vertices by rotation
std::vector<glm::vec2> transformVertices(const std::vector<glm::vec2> &vertices, float angle)
{
    std::vector<glm::vec2> transformed;
    float cosA = std::cos(angle);
    float sinA = std::sin(angle);

    for (const auto &v : vertices)
    {
        glm::vec2 rotated;
        rotated.x = v.x * cosA - v.y * sinA;
        rotated.y = v.x * sinA + v.y * cosA;
        transformed.push_back(rotated);
    }

    return transformed;
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Transform triangle vertices
    std::vector<glm::vec2> transformedVertices = transformVertices(triangleVertices, angle);

    // Compute OBB
    OBB obb = computeOBB(transformedVertices);

    // Draw triangle (filled)
    glColor3f(0.3f, 0.6f, 0.9f);
    glBegin(GL_TRIANGLES);
    for (const auto &v : transformedVertices)
    {
        glVertex2f(v.x, v.y);
    }
    glEnd();

    // Draw triangle outline
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    for (const auto &v : transformedVertices)
    {
        glVertex2f(v.x, v.y);
    }
    glEnd();

    // Draw OBB
    std::vector<glm::vec2> obbCorners = getOBBCorners(obb);
    glColor3f(1.0f, 0.2f, 0.2f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    for (const auto &corner : obbCorners)
    {
        glVertex2f(corner.x, corner.y);
    }
    glEnd();

    // Draw OBB axes
    glLineWidth(1.5f);
    // X-axis (red)
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glm::vec2 axisEnd = obb.center + obb.axes[0] * obb.halfExtents.x;
    glVertex2f(obb.center.x, obb.center.y);
    glVertex2f(axisEnd.x, axisEnd.y);
    glEnd();

    // Y-axis (green)
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_LINES);
    axisEnd = obb.center + obb.axes[1] * obb.halfExtents.y;
    glVertex2f(obb.center.x, obb.center.y);
    glVertex2f(axisEnd.x, axisEnd.y);
    glEnd();

    // Draw center point
    glColor3f(1.0f, 1.0f, 0.0f);
    glPointSize(6.0f);
    glBegin(GL_POINTS);
    glVertex2f(obb.center.x, obb.center.y);
    glEnd();

    glutSwapBuffers();
}

void timer(int value)
{
    angle += 0.02f;
    if (angle > 2.0f * M_PI)
    {
        angle -= 2.0f * M_PI;
    }

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // ~60 FPS
}

void init()
{
    glClearColor(0.95f, 0.95f, 0.95f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("2D OBB with Separating Axis Theorem");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}