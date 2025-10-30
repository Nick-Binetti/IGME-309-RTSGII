// Usage:
// Hold down the number keys, 1-7, to select one or multiple circles.
// While circle(s) are selected, use the left mouse button to translate and use the right mouse button to rotate.

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <iostream>
#include <vector>
#include <math.h>
using namespace std;

#define MAX_NUM_CIRCLE 7
#define CIRCLE_RADIUM 2.0

int win_width = 600, win_height = 600;
float canvas_width = 20.0f;
float canvas_height = 20.0f;

// Scene Graph Node
class SceneNode
{
public:
    int id;
    float color[3];
    float translation[2];
    float rotation;
    float radius;

    SceneNode *parent;
    vector<SceneNode *> children;

    SceneNode(int nodeId, float r)
    {
        id = nodeId;
        radius = r;
        color[0] = color[1] = color[2] = 0.0f;
        translation[0] = translation[1] = 0.0f;
        rotation = 0.0f;
        parent = nullptr;
    }

    void addChild(SceneNode *child)
    {
        children.push_back(child);
        child->parent = this;
    }

    void draw()
    {
        glPushMatrix();

        // Apply this node's transformations
        glTranslatef(translation[0], translation[1], 0.0f);
        glRotatef(rotation, 0.0f, 0.0f, 1.0f);

        // Draw this circle
        glColor3fv(color);
        glLineWidth(3.0f);
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i <= 100; i++)
            glVertex2f(radius * cosf(3.14 * 2 / 100 * i), radius * sinf(3.14 * 2 / 100 * i));
        glEnd();

        // Draw all children
        for (size_t i = 0; i < children.size(); i++)
        {
            children[i]->draw();
        }

        glPopMatrix();
    }
};

// Global variables
bool keyStates[256];
int buttonState;
float curMouse[2];
float preMouse[2];

SceneNode *root = nullptr;
SceneNode *nodes[MAX_NUM_CIRCLE];

void buildSceneGraph()
{
    // Create all nodes
    for (int i = 0; i < MAX_NUM_CIRCLE; i++)
    {
        float r = CIRCLE_RADIUM * (MAX_NUM_CIRCLE - i) / MAX_NUM_CIRCLE;
        nodes[i] = new SceneNode(i, r);
    }

    // Build the hierarchy matching the original code's push/pop:
    // Circle 0 (root)
    //   └─ Circle 1
    //       ├─ Circle 2 → Circle 3 (first branch)
    //       └─ Circle 4 → Circle 5 (second branch)

    root = nodes[0];
    nodes[0]->addChild(nodes[1]);
    nodes[1]->addChild(nodes[2]);
    nodes[2]->addChild(nodes[3]);
    nodes[1]->addChild(nodes[4]);
    nodes[4]->addChild(nodes[5]);

    // Circle 6 can be added as another branch if needed
    if (MAX_NUM_CIRCLE > 6)
    {
        nodes[0]->addChild(nodes[6]);
    }
}

void init(void)
{
    for (int i = 0; i < 256; i++)
    {
        keyStates[i] = false;
    }

    buttonState = -1;
    buildSceneGraph();
}

void display(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Draw the entire scene graph starting from root
    if (root)
    {
        root->draw();
    }

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    win_width = w;
    win_height = h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-canvas_width / 2.0f, canvas_width / 2.0f, -canvas_height / 2.0f, canvas_height / 2.0f);
    glViewport(0, 0, (GLsizei)win_width, (GLsizei)win_height);

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    if (key == 27) // 'esc' key
        exit(0);

    for (unsigned char i = '1'; i <= '7'; i++)
    {
        if (key == i)
        {
            int idx = i - '1';
            if (idx < MAX_NUM_CIRCLE)
            {
                keyStates[i] = true;
                nodes[idx]->color[0] = 1.0f;
                nodes[idx]->color[1] = 0.0f;
                nodes[idx]->color[2] = 0.0f;
            }
        }
    }
    glutPostRedisplay();
}

void keyboardUp(unsigned char key, int x, int y)
{
    for (unsigned char i = '1'; i <= '7'; i++)
    {
        if (key == i)
        {
            int idx = i - '1';
            if (idx < MAX_NUM_CIRCLE)
            {
                keyStates[i] = false;
                nodes[idx]->color[0] = 0.0f;
                nodes[idx]->color[1] = 0.0f;
                nodes[idx]->color[2] = 0.0f;
            }
        }
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN)
    {
        buttonState = button;
        curMouse[0] = ((float)x / win_width - 0.5f) * canvas_width;
        curMouse[1] = ((float)(win_height - y) - 0.5f) / win_height * canvas_height;
        preMouse[0] = curMouse[0];
        preMouse[1] = curMouse[1];
    }
    else if (state == GLUT_UP)
        buttonState = -1;
}

void motion(int x, int y)
{
    curMouse[0] = ((float)x / win_width - 0.5f) * canvas_width;
    curMouse[1] = ((float)(win_height - y) - 0.5f) / win_height * canvas_height;

    if (buttonState == GLUT_LEFT_BUTTON)
    {
        for (unsigned char i = '1'; i <= '7'; i++)
        {
            if (keyStates[i])
            {
                int idx = i - '1';
                if (idx < MAX_NUM_CIRCLE)
                {
                    nodes[idx]->translation[0] += curMouse[0] - preMouse[0];
                    nodes[idx]->translation[1] += curMouse[1] - preMouse[1];
                }
            }
        }
        glutPostRedisplay();
    }
    else if (buttonState == GLUT_RIGHT_BUTTON)
    {
        for (unsigned char i = '1'; i <= '7'; i++)
        {
            if (keyStates[i])
            {
                int idx = i - '1';
                if (idx < MAX_NUM_CIRCLE)
                {
                    nodes[idx]->rotation += curMouse[0] - preMouse[0];
                }
            }
        }
        glutPostRedisplay();
    }

    preMouse[0] = curMouse[0];
    preMouse[1] = curMouse[1];
}

void cleanup()
{
    for (int i = 0; i < MAX_NUM_CIRCLE; i++)
    {
        delete nodes[i];
    }
}

int main(int argc, char *argv[])
{
    init();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(win_width, win_height);
    glutCreateWindow("2D Transformation Tree - Scene Graph");

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    atexit(cleanup);

    glutMainLoop();
    return 0;
}