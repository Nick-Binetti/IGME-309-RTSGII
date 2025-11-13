#include <GL/glut.h>
#include <cmath>
#include <vector>

#define M_PI 3.1415926f

// Hermite basis functions
float h1(float t) { return 2 * t * t * t - 3 * t * t + 1; }
float h2(float t) { return -2 * t * t * t + 3 * t * t; }
float h3(float t) { return t * t * t - 2 * t * t + t; }
float h4(float t) { return t * t * t - t * t; }

// 3D Point structure
struct Point3D
{
    float x, y, z;
    Point3D(float _x = 0, float _y = 0, float _z = 0) : x(_x), y(_y), z(_z) {}
};

// Hermite curve segments for a closed loop
struct HermiteSegment
{
    Point3D p0, p1, t0, t1;
};

std::vector<HermiteSegment> segments = {
    {{0, 0, 0}, {8, 2, 0}, {6, 0, 0}, {6, 0, 0}},
    {{8, 2, 0}, {10, 8, 4}, {0, 6, 4}, {0, 6, 4}},
    {{10, 8, 4}, {4, 12, 8}, {-6, 4, 4}, {-6, 4, 4}},
    {{4, 12, 8}, {-2, 10, 4}, {-6, -2, -4}, {-6, -2, -4}},
    {{-2, 10, 4}, {-4, 4, 0}, {0, -6, -4}, {0, -6, -4}},
    {{-4, 4, 0}, {0, 0, 0}, {6, -4, 0}, {6, -4, 0}}};

// Animation parameters
float animTime = 0.0f;
float animSpeed = 0.15f;
bool useEasing = false;
std::vector<Point3D> curvePoints;

// Camera parameters
float camAngleX = 20.0f;
float camAngleY = 45.0f;
float camDistance = 25.0f;

// Calculate point on Hermite curve segment
Point3D hermitePoint(const HermiteSegment &seg, float t)
{
    Point3D p;
    p.x = h1(t) * seg.p0.x + h2(t) * seg.p1.x + h3(t) * seg.t0.x + h4(t) * seg.t1.x;
    p.y = h1(t) * seg.p0.y + h2(t) * seg.p1.y + h3(t) * seg.t0.y + h4(t) * seg.t1.y;
    p.z = h1(t) * seg.p0.z + h2(t) * seg.p1.z + h3(t) * seg.t0.z + h4(t) * seg.t1.z;
    return p;
}

// Get point on the complete closed loop
Point3D getLoopPoint(float t)
{
    int numSegments = segments.size();
    float segmentT = t * numSegments;
    int segmentIndex = (int)segmentT % numSegments;
    float localT = segmentT - (int)segmentT;
    return hermitePoint(segments[segmentIndex], localT);
}

// ****************************************************************
// Write your code here.
// Add functions for the cubic Bezier function.
Point3D cubicBezier(float t, float p0, float p1, float p2, float p3, float animTime)
{
    float u = 1.0f - t;
    return u * u * u * p0 + 3 * u * u * t * p1 + 3 * u * t * t * p2 + t * t * t * p3;
}

// ****************************************************************

// Initialize the curve points
void initCurve()
{
    curvePoints.clear();
    int pointsPerSegment = 50;
    for (const auto &seg : segments)
    {
        for (int i = 0; i <= pointsPerSegment; i++)
        {
            float t = i / (float)pointsPerSegment;
            curvePoints.push_back(hermitePoint(seg, t));
        }
    }
}

void init()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLfloat lightPos[] = {5.0f, 5.0f, 5.0f, 1.0f};
    GLfloat lightAmb[] = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat lightDiff[] = {0.8f, 0.8f, 0.8f, 1.0f};

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiff);

    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    initCurve();
}

void drawBox(float size)
{
    glutSolidCube(size);
}

void drawCurve()
{
    glDisable(GL_LIGHTING);
    glColor4f(1.0f, 1.0f, 0.0f, 0.4f); // Yellow with 40% opacity
    glLineWidth(3.0f);

    glBegin(GL_LINE_STRIP);
    for (const auto &p : curvePoints)
    {
        glVertex3f(p.x, p.y, p.z);
    }
    glEnd();

    glEnable(GL_LIGHTING);
}

void drawAxes()
{
    glDisable(GL_LIGHTING);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    // X axis - red
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0, 0, 0);
    glVertex3f(3, 0, 0);
    // Y axis - green
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 3, 0);
    // Z axis - blue
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 3);
    glEnd();
    glEnable(GL_LIGHTING);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Set up camera
    gluLookAt(camDistance * cos(camAngleY * M_PI / 180) * cos(camAngleX * M_PI / 180),
              camDistance * sin(camAngleX * M_PI / 180),
              camDistance * sin(camAngleY * M_PI / 180) * cos(camAngleX * M_PI / 180),
              3.0, 6.0, 4.0,
              0, 1, 0);

    drawAxes();
    drawCurve();

    // Draw moving box
    float t = fmod(animTime, 1.0f);

    // ****************************************************************
    // Write your code here.
    // Hints:
    // 1. useEasing -> a flag for current easing state.
    // 2. apply necessary changes on t.
    if (useEasing)
    {
        
    }

    // ****************************************************************
    Point3D boxPos = getLoopPoint(t);

    glPushMatrix();
    glTranslatef(boxPos.x, boxPos.y, boxPos.z);
    glColor3f(0.3f, 0.7f, 1.0f);
    drawBox(0.5f);
    glPopMatrix();

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void timer(int value)
{
    animTime += 0.01f * animSpeed;
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27:
        exit(0);
        break; // ESC
    case ' ':
        useEasing = !useEasing;
        printf("Easing: %s\n", useEasing ? "ON" : "OFF");
        break;
    case '+':
        animSpeed += 0.1f;
        break;
    case '-':
        animSpeed = fmax(0.1f, animSpeed - 0.1f);
        break;
    case 'r':
        animTime = 0.0f;
        break;
    }
}

void specialKeys(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_LEFT:
        camAngleY -= 5.0f;
        break;
    case GLUT_KEY_RIGHT:
        camAngleY += 5.0f;
        break;
    case GLUT_KEY_UP:
        camAngleX += 5.0f;
        break;
    case GLUT_KEY_DOWN:
        camAngleX -= 5.0f;
        break;
    }
    camAngleX = fmax(-89.0f, fmin(89.0f, camAngleX));
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Hermite Curve and Bezier Animation");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}
