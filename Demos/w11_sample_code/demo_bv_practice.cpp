// robot_transform_tree.cpp
// Compile: g++ robot_transform_tree.cpp -o robot -lGL -lGLU -lglut

#if APPLE
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif
#include <cmath>
#include <cstdio>
#include <cstring>


// ---------- Configuration ----------
const int NUM_PARTS = 16;
float angles[NUM_PARTS]; // rotation angle for each body part (degrees)
int selected = 0;        // selected part index

// ---------- Part indices & names ----------
enum Part {
    RIGHT_HAND = 0,
    RIGHT_FOREARM = 1,
    RIGHT_UPPERARM = 2,
    LEFT_HAND = 3,
    LEFT_FOREARM = 4,
    LEFT_UPPERARM = 5,
    HEAD = 6,
    NECK = 7,
    UPPER_BODY = 8,
    LOWER_BODY = 9,
    RIGHT_FOOT = 10,
    RIGHT_SHIN = 11,
    RIGHT_THIGH = 12,
    LEFT_FOOT = 13,
    LEFT_SHIN = 14,
    LEFT_THIGH = 15
};

const char* partNames[NUM_PARTS] = {
    "Right Hand","Right Forearm","Right Upper Arm",
    "Left Hand","Left Forearm","Left Upper Arm",
    "Head","Neck","Upper Body","Lower Body",
    "Right Foot","Right Shin","Right Thigh",
    "Left Foot","Left Shin","Left Thigh"
};

// ---------- Utility drawing helpers ----------
void drawRect(float w, float h) {
    glBegin(GL_QUADS);
    glVertex2f(-w / 2.0f, 0.0f);
    glVertex2f(w / 2.0f, 0.0f);
    glVertex2f(w / 2.0f, h);
    glVertex2f(-w / 2.0f, h);
    glEnd();
}

void drawCircle(float r, int segments = 32) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0, 0);
    for (int i = 0; i <= segments; i++) {
        float a = 2.0f * 3.1415 * float(i) / float(segments);
        glVertex2f(cos(a) * r, sin(a) * r);
    }
    glEnd();
}

void drawJointMarker() {
    // small filled circle
    drawCircle(6.0f / 100.0f); // scaled later using glScalef
}

// ---------- Screen info ----------
int winW = 800, winH = 700;

// Draw on-screen text using bitmap fonts
void drawString(float x, float y, const char* s) {
    glRasterPos2f(x, y);
    for (const char* c = s; *c; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
}

// ---------- Robot drawing (hierarchical) ----------
/*
Coordinate system chosen: All lengths are expressed in units that fit nicely in [-1,1] after we scale.
We'll use the following approximate sizes (in OpenGL units):
- Lower body height = 0.35
- Upper body height = 0.18
- Neck height = 0.06
- Head size ~ hexagon (we'll draw a circle)
- Arms: upper arm length 0.25, forearm 0.22, hand 0.08
- Thigh 0.28, shin 0.22, foot 0.12
*/
void drawRobot() {
    // 1. Move robot so it's centered
    glPushMatrix();
    // scale for nicer pixel independent sizing
    glScalef(1.0f, 1.0f, 1.0f);

    // ------ LOWER BODY (index LOWER_BODY) ------
    glPushMatrix();
    // lower body rectangle centered at origin with top at y = lowerBodyHeight
    float lowerBodyHeight = 0.35f;
    float lowerBodyW = 0.28f;
    // rotate lower body
    glRotatef(angles[LOWER_BODY], 0, 0, 1);

    // draw lower body
    glColor3f(0.5f, 0.5f, 0.6f);
    drawRect(lowerBodyW, lowerBodyHeight);

    // hip joint markers (two hips left/right)
    // left hip
    glPushMatrix();
    glTranslatef(-lowerBodyW / 2.0f + 0.05f, 0.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 0.0f); drawCircle(0.025f);
    glPopMatrix();
    // right hip
    glPushMatrix();
    glTranslatef(lowerBodyW / 2.0f - 0.05f, 0.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 0.0f); drawCircle(0.025f);
    glPopMatrix();

    // ------ RIGHT LEG (parent = lower body) ------
    glPushMatrix();
    // translate to right hip pivot
    glTranslatef(lowerBodyW / 2.0f - 0.05f, 0.0f, 0.0f);
    // rotate thigh
    glRotatef(angles[RIGHT_THIGH], 0, 0, 1);
    // draw thigh rectangle upward from pivot
    glColor3f(0.8f, 0.5f, 0.6f);
    drawRect(0.08f, 0.28f); // thigh

    // knee joint
    glTranslatef(0.0f, 0.28f, 0.0f);
    glColor3f(0.0f, 0.0f, 0.0f); drawCircle(0.02f);

    // shin (child of thigh)
    glPushMatrix();
    glRotatef(angles[RIGHT_SHIN], 0, 0, 1);
    glColor3f(0.9f, 0.6f, 0.6f);
    drawRect(0.07f, 0.22f); // shin
    // foot pivot at end of shin
    glTranslatef(0.0f, 0.22f, 0.0f);
    glColor3f(0.0f, 0.0f, 0.0f); drawCircle(0.018f);
    // foot (child of shin)
    glPushMatrix();
    glRotatef(angles[RIGHT_FOOT], 0, 0, 1);
    glTranslatef(0.0f, 0.02f, 0.0f);
    glColor3f(0.3f, 0.6f, 0.9f);
    glBegin(GL_QUADS);
    glVertex2f(-0.06f, 0.0f); glVertex2f(0.06f, 0.0f);
    glVertex2f(0.06f, 0.06f); glVertex2f(-0.06f, 0.06f);
    glEnd();
    glPopMatrix();
    glPopMatrix();

    glPopMatrix(); // end right leg

    // ------ LEFT LEG (parent = lower body) ------
    glPushMatrix();
    // translate to left hip pivot
    glTranslatef(-lowerBodyW / 2.0f + 0.05f, 0.0f, 0.0f);
    glRotatef(angles[LEFT_THIGH], 0, 0, 1);
    glColor3f(0.6f, 0.7f, 0.5f);
    drawRect(0.08f, 0.28f); // thigh

    // knee
    glTranslatef(0.0f, 0.28f, 0.0f);
    glColor3f(0.0f, 0.0f, 0.0f); drawCircle(0.02f);

    // shin
    glPushMatrix();
    glRotatef(angles[LEFT_SHIN], 0, 0, 1);
    glColor3f(0.7f, 0.8f, 0.6f);
    drawRect(0.07f, 0.22f); // shin
    // foot
    glTranslatef(0.0f, 0.22f, 0.0f);
    glColor3f(0.0f, 0.0f, 0.0f); drawCircle(0.018f);
    glPushMatrix();
    glRotatef(angles[LEFT_FOOT], 0, 0, 1);
    glTranslatef(0.0f, 0.02f, 0.0f);
    glColor3f(0.2f, 0.9f, 0.4f);
    glBegin(GL_QUADS);
    glVertex2f(-0.06f, 0.0f); glVertex2f(0.06f, 0.0f);
    glVertex2f(0.06f, 0.06f); glVertex2f(-0.06f, 0.06f);
    glEnd();
    glPopMatrix();
    glPopMatrix();

    glPopMatrix(); // end left leg

    // ------ UPPER BODY (child of lower body): translate to top of lower body ------
    glPushMatrix();
    float lowerTopY = lowerBodyHeight;
    glTranslatef(0.0f, lowerTopY, 0.0f);
    glRotatef(angles[UPPER_BODY], 0, 0, 1);
    // draw upper body
    float upperW = 0.65f, upperH = 0.18f;
    glColor3f(0.2f, 0.8f, 0.8f);
    drawRect(upperW, upperH);

    // neck pivot (center top)
    glPushMatrix();
    glTranslatef(0.0f, upperH, 0.0f);
    glColor3f(0.0f, 0.0f, 0.0f); drawCircle(0.02f);
    glPopMatrix();

    // ------ RIGHT ARM (attach to right side of upper body) ------
    glPushMatrix();
    // place pivot at right shoulder
    glTranslatef(upperW / 2.0f - 0.08f, 0.05f, 0.0f);
    glRotatef(angles[RIGHT_UPPERARM], 0, 0, 1);
    // upper arm
    glColor3f(0.7f, 0.4f, 0.2f);
    drawRect(0.08f, 0.25f);

    // elbow pivot
    glTranslatef(0.0f, 0.25f, 0.0f);
    glColor3f(0.0f, 0.0f, 0.0f); drawCircle(0.015f);

    // forearm
    glPushMatrix();
    glRotatef(angles[RIGHT_FOREARM], 0, 0, 1);
    glColor3f(0.9f, 0.5f, 0.3f);
    drawRect(0.07f, 0.22f);

    // hand pivot at end of forearm
    glTranslatef(0.0f, 0.22f, 0.0f);
    glColor3f(0.0f, 0.0f, 0.0f); drawCircle(0.012f);

    // hand
    glPushMatrix();
    glRotatef(angles[RIGHT_HAND], 0, 0, 1);
    glTranslatef(0.0f, 0.05f, 0.0f);
    glColor3f(0.2f, 0.5f, 0.9f);
    glBegin(GL_QUADS);
    glVertex2f(-0.045f, 0.0f); glVertex2f(0.045f, 0.0f);
    glVertex2f(0.045f, 0.09f); glVertex2f(-0.045f, 0.09f);
    glEnd();
    glPopMatrix();

    glPopMatrix();
    glPopMatrix();

    // ------ LEFT ARM ------
    glPushMatrix();
    // place pivot at left shoulder
    glTranslatef(-upperW / 2.0f + 0.08f, 0.05f, 0.0f);
    glRotatef(angles[LEFT_UPPERARM], 0, 0, 1);
    // upper arm
    glColor3f(0.3f, 0.6f, 0.2f);
    drawRect(0.08f, 0.25f);

    // elbow pivot
    glTranslatef(0.0f, 0.25f, 0.0f);
    glColor3f(0.0f, 0.0f, 0.0f); drawCircle(0.015f);

    // forearm
    glPushMatrix();
    glRotatef(angles[LEFT_FOREARM], 0, 0, 1);
    glColor3f(0.4f, 0.7f, 0.3f);
    drawRect(0.07f, 0.22f);

    // hand pivot at end of forearm
    glTranslatef(0.0f, 0.22f, 0.0f);
    glColor3f(0.0f, 0.0f, 0.0f); drawCircle(0.012f);

    // hand
    glPushMatrix();
    glRotatef(angles[LEFT_HAND], 0, 0, 1);
    glTranslatef(0.0f, 0.05f, 0.0f);
    glColor3f(0.9f, 0.2f, 0.5f);
    glBegin(GL_QUADS);
    glVertex2f(-0.045f, 0.0f); glVertex2f(0.045f, 0.0f);
    glVertex2f(0.045f, 0.09f); glVertex2f(-0.045f, 0.09f);
    glEnd();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();

    // ------ NECK & HEAD (children of upper body) ------
    glPushMatrix();
    glTranslatef(0.0f, upperH, 0.0f);
    glRotatef(angles[NECK], 0, 0, 1);
    // neck
    glColor3f(0.8f, 0.8f, 0.2f);
    drawRect(0.06f, 0.06f);

    // head pivot at top of neck
    glTranslatef(0.0f, 0.06f, 0.0f);
    glColor3f(0.0f, 0.0f, 0.0f); drawCircle(0.02f);

    // head (child of neck)
    glPushMatrix();
    glRotatef(angles[HEAD], 0, 0, 1);
    glTranslatef(0.0f, 0.14f, 0.0f);
    glColor3f(0.2f, 0.6f, 0.2f);
    drawCircle(0.12f, 40); // head approximated as circle
    glPopMatrix();
    glPopMatrix();

    glPopMatrix(); // end upper body (child of lower body)

    glPopMatrix(); // end lower body
    glPopMatrix(); // restore
}

// ---------- Display callback ----------
void display() {
    glClearColor(0.95f, 0.95f, 0.98f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Center and scale robot to viewport
    glTranslatef(0.0f, -0.05f, 0.0f);
    // scale up so robot fills window better
    float scale = 1.2f;
    glScalef(scale, scale, 1.0f);

    // Draw robot
    drawRobot();

    // Overlay UI text in screen coordinates
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, winW, 0, winH);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    // Label showing selected part and angle
    char buf[256];
    sprintf(buf, "Selected [%d/%d]: %s   Angle: %.1f deg", selected + 1, NUM_PARTS, partNames[selected], angles[selected]);
    glColor3f(0.0f, 0.0f, 0.0f);
    // draw from bottom-left
    glRasterPos2i(10, winH - 18);
    for (char* c = buf; *c; ++c) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);

    // instruction lines
    const char* instrs[] = {
        "Controls: n = next part, p = previous part, Left/Right arrows = rotate selected (-/+ 5deg)",
        "r = reset all, q or Esc = quit"
    };
    glRasterPos2i(10, winH - 36);
    for (const char* s = instrs[0]; *s; ++s) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *s);
    glRasterPos2i(10, winH - 54);
    for (const char* s = instrs[1]; *s; ++s) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *s);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    // Highlight selected joint by drawing a small red circle on top of the corresponding pivot
    // We'll approximate where the joint is by re-drawing the robot but marking transforms for the selected part.
    // Simpler approach: re-traverse the hierarchy and draw red markers at pivot points with same transforms.
    // (Do a simpler traversal similar to drawRobot but only draw small red markers at the pivot locations)

    // Small helper lambda inside function not allowed in plain C++98, so just implement inline traversal here.

    // For simplicity we will draw a red marker next to the robot corresponding to the selected part index:
    // Compute small on-screen legend marker
    glColor3f(1.0f, 0.0f, 0.0f);
    float legendX = -0.9f, legendY = 0.9f - 0.03f * selected;
    glPushMatrix();
    glTranslatef(legendX, legendY, 0.0f);
    glScalef(0.03f, 0.03f, 1.0f);
    drawCircle(0.6f);
    glPopMatrix();

    glutSwapBuffers();
}

// ---------- Window reshape ----------
void reshape(int w, int h) {
    winW = w; winH = h;
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Use simple orthographic projection covering -1..1 in x, -1..1 in y adjusted for aspect
    float aspect = (float)w / (float)h;
    if (aspect >= 1.0f) {
        // wide window
        glOrtho(-1.2f * aspect, 1.2f * aspect, -1.2f, 1.2f, -1.0f, 1.0f);
    }
    else {
        glOrtho(-1.2f, 1.2f, -1.2f / aspect, 1.2f / aspect, -1.0f, 1.0f);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// ---------- Input handling ----------
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 27: // ESC
    case 'q':
    case 'Q':
        exit(0);
        break;
    case 'n':
    case 'N':
        selected = (selected + 1) % NUM_PARTS;
        glutPostRedisplay();
        break;
    case 'p':
    case 'P':
        selected = (selected - 1 + NUM_PARTS) % NUM_PARTS;
        glutPostRedisplay();
        break;
    case 'r':
    case 'R':
        for (int i = 0; i < NUM_PARTS; i++) angles[i] = 0.0f;
        glutPostRedisplay();
        break;
    }
}

void specialKey(int key, int x, int y) {
    const float delta = 5.0f; // degrees per press
    if (key == GLUT_KEY_LEFT) {
        angles[selected] -= delta;
        if (angles[selected] < -360.0f) angles[selected] += 360.0f;
        glutPostRedisplay();
    }
    else if (key == GLUT_KEY_RIGHT) {
        angles[selected] += delta;
        if (angles[selected] > 360.0f) angles[selected] -= 360.0f;
        glutPostRedisplay();
    }
}

// ---------- Initialization ----------
void init() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // initial angles all zero
    for (int i = 0; i < NUM_PARTS; i++) angles[i] = 0.0f;
}

// ---------- main ----------
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(winW, winH);
    glutCreateWindow("2D Hierarchical Robot: Select parts and rotate (n/p, arrows)");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKey);

    glutMainLoop();
    return 0;
}
