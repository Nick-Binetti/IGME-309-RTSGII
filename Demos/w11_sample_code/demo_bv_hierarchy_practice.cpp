#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>

#define M_PI 3.1415926f

// Constants
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float BOX_SIZE = 15.0f;

// Box structure for geometry
struct Box
{
    float x, y, size;
    Box(float cx, float cy, float s) : x(cx), y(cy), size(s) {}
};

// Circle structure
struct Circle
{
    float x, y, radius;
    std::vector<Circle *> children;
    std::vector<int> boxIndices; // Indices of boxes in this leaf
    bool isLeaf;
    bool isColliding;

    Circle(float cx, float cy, float r, bool leaf = false)
        : x(cx), y(cy), radius(r), isLeaf(leaf), isColliding(false) {}
};

// Global variables
float mouseX = 0.0f;
float mouseY = 0.0f;
Circle *root = nullptr;
std::vector<Box> boxes;
bool leafHit = false;

// Convert screen coordinates to OpenGL coordinates
void screenToGL(int sx, int sy, float &gx, float &gy)
{
    gx = (float)sx;
    gy = (float)(WINDOW_HEIGHT - sy);
}

// Check collision between two circles
bool checkCircleCollision(float x1, float y1, float r1, float x2, float y2, float r2)
{
    float dx = x2 - x1;
    float dy = y2 - y1;
    float distance = sqrt(dx * dx + dy * dy);
    return distance <= (r1 + r2);
}

// ==============================================================================
// PRACTICE 1: Get cursor bounding circle radius from box vertices
// ==============================================================================
float getCursorBoundingCircle(float x1, float y1, float x2, float y2,
                              float x3, float y3, float x4, float y4)
{
    //1.
    std::vector<float> vertices = {x1,y1,x2,y2,x3,y3,x4,y4};
    float centerX = (x1 + x2 + x3 + x4) / 4.0;
    float centerY = (y1 + y2 + y3 + y4) / 4.0;
    //2.
    float maxDist = 0.0f;

    //3.
    float dx = x1 - centerX;
    float dy = y1 - centerY; 
    
}
// OBJECTIVE: Calculate the radius of a bounding circle that encloses a box
// INPUT: 4 vertices of the cursor box (x1,y1), (x2,y2), (x3,y3), (x4,y4)
// OUTPUT: radius of the bounding circle
//
// INSTRUCTIONS:
// Step 1: Calculate the center point of the box
//         Hint: The center is the average of all 4 vertices
//         centerX = (x1 + x2 + x3 + x4) / 4.0
//         centerY = (y1 + y2 + y3 + y4) / 4.0
//
// Step 2: Initialize a variable to track the maximum distance
         
//
// Step 3: For each of the 4 vertices, calculate the distance from center
//         Use the distance formula: distance = sqrt((x - centerX)^2 + (y - centerY)^2)
//         Keep track of the maximum distance found
//
// Step 4: Return the maximum distance as the radius
//         This ensures all vertices are inside the bounding circle
// ==============================================================================
float getCursorBoundingCircleRadius(float x1, float y1, float x2, float y2,
                                    float x3, float y3, float x4, float y4)
{
    // STEP 1: Calculate center of the box
    // TODO: Compute centerX and centerY

    // STEP 2: Initialize maximum distance tracker
    // TODO: Create a variable to store the maximum distance

    // STEP 3: Calculate distance from center to each vertex
    // TODO: For vertex 1 (x1, y1)

    // TODO: For vertex 2 (x2, y2)

    // TODO: For vertex 3 (x3, y3)

    // TODO: For vertex 4 (x4, y4)

    // STEP 4: Return the radius
    // TODO: Return the maximum distance
    return 0.1;
}

// ==============================================================================
// PRACTICE 2: Collision detection using bounding circle hierarchy
// ==============================================================================
// OBJECTIVE: Detect collision between cursor and geometry using BVH tree
// INPUT:
//   - node: current node in the bounding circle tree
//   - cursorX, cursorY: position of the cursor
//   - cursorRadius: radius of the cursor's bounding circle
//   - leafHitFlag: reference to flag that tracks if we hit actual geometry
// OUTPUT: true if collision detected, false otherwise
//
// ALGORITHM OVERVIEW:
// This is a recursive tree traversal with early pruning
// - Start at root and check collision with cursor circle
// - If no collision at parent, skip all children (PRUNING!)
// - If collision at leaf, we hit actual geometry
// - Recursively check all children if parent collides
//
// INSTRUCTIONS:
// Step 1: Handle null node (base case)
//         If node is null, return false
//
// Step 2: Calculate distance between cursor circle and current bounding circle
//         Use distance formula: d = sqrt((node.x - cursorX)^2 + (node.y - cursorY)^2)
//
// Step 3: Check if circles collide (circle-to-circle test)
//         Two circles collide if: distance <= (radius1 + radius2)
//         Store result in node->isColliding
//
// [Optional] Step 4: Early pruning - if no collision, return false
//         This is the key optimization of BVH!
//         If parent doesn't collide, children won't either
//
// Step 5: Check if this is a leaf node
//         If it's a leaf AND colliding, set leafHitFlag = true
//         A leaf contains actual geometry, so this is a real hit
//
// Step 6: Recursively check all children
//         For each child, call collisionDetection recursively
//         Track if any child reports a hit
//
// Step 7: Return collision result
//         Return true if any child had a collision
// ==============================================================================
bool collisionDetection(Circle *node, float cursorX, float cursorY, float cursorRadius, bool &leafHitFlag)
{
    // STEP 1: Base case - null node
    // TODO: Check if node is null, return false if so

    // STEP 2: Calculate distance between circles
    // TODO: Compute dx = node->x - cursorX
    // TODO: Compute dy = node->y - cursorY
    // TODO: Compute distance = sqrt(dx^2 + dy^2)

    // STEP 3: Circle-to-circle collision test
    // TODO: Check if distance <= (node->radius + cursorRadius)
    // TODO: Store result in node->isColliding

    // STEP 4: Early pruning optimization
    // TODO: If node->isColliding is false, return false
    // This prunes the entire subtree! Key to BVH efficiency

    // STEP 5: Check for leaf node collision
    // TODO: If node->isLeaf is true, set leafHitFlag = true and return true
    // This means we hit actual geometry!

    // STEP 6: Recursively check children
    // TODO: Initialize childHit = false
    // TODO: Loop through all children in node->children
    // TODO: For each child, call collisionDetection recursively
    // TODO: If any child returns true, set childHit = true

    // STEP 7: Return result
    // TODO: Return childHit

    return false;
}

// Reset collision states
void resetCollisions(Circle *node)
{
    if (!node)
        return;
    node->isColliding = false;
    for (Circle *child : node->children)
    {
        resetCollisions(child);
    }
}

// Draw a circle outline
void drawCircle(float cx, float cy, float radius, bool filled = false)
{
    int segments = 50;
    if (filled)
        glBegin(GL_TRIANGLE_FAN);
    else
        glBegin(GL_LINE_LOOP);

    for (int i = 0; i < segments; i++)
    {
        float theta = 2.0f * M_PI * float(i) / float(segments);
        float x = radius * cosf(theta);
        float y = radius * sinf(theta);
        glVertex2f(cx + x, cy + y);
    }
    glEnd();
}

// Draw text on screen
void drawText(float x, float y, const std::string &text)
{
    glRasterPos2f(x, y);
    for (char c : text)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

// Recursively draw the hierarchy
void drawHierarchy(Circle *node, bool forceRed)
{
    if (!node)
        return;

    // Draw geometry boxes if this is a leaf node
    if (node->isLeaf)
    {
        glColor3f(0.3f, 0.5f, 0.8f); // Solid blue color for geometry
        glBegin(GL_QUADS);
        for (int idx : node->boxIndices)
        {
            float half = boxes[idx].size / 2.0f;
            float x = boxes[idx].x;
            float y = boxes[idx].y;
            glVertex2f(x - half, y - half);
            glVertex2f(x + half, y - half);
            glVertex2f(x + half, y + half);
            glVertex2f(x - half, y + half);
        }
        glEnd();
    }

    // Determine color for bounding circle
    if (forceRed || node->isColliding)
    {
        glColor3f(1.0f, 0.0f, 0.0f); // Red
    }
    else
    {
        if (node->isLeaf)
        {
            glColor3f(0.0f, 1.0f, 0.0f); // Green for leaves
        }
        else
        {
            glColor3f(0.5f, 0.5f, 1.0f); // Light blue for internal nodes
        }
    }

    glLineWidth(node->isLeaf ? 2.0f : 1.5f);
    drawCircle(node->x, node->y, node->radius);

    // Draw children
    for (Circle *child : node->children)
    {
        drawHierarchy(child, forceRed);
    }
}

// Calculate bounding circle for a set of boxes
void calculateBoundingCircle(const std::vector<int> &boxIndices, float &cx, float &cy, float &radius)
{
    if (boxIndices.empty())
        return;

    // Find bounding box
    float minX = 1e6, maxX = -1e6, minY = 1e6, maxY = -1e6;
    for (int idx : boxIndices)
    {
        float half = boxes[idx].size / 2.0f;
        minX = std::min(minX, boxes[idx].x - half);
        maxX = std::max(maxX, boxes[idx].x + half);
        minY = std::min(minY, boxes[idx].y - half);
        maxY = std::max(maxY, boxes[idx].y + half);
    }

    // Center of bounding circle
    cx = (minX + maxX) / 2.0f;
    cy = (minY + maxY) / 2.0f;

    // Radius is distance to farthest corner
    float dx = maxX - cx;
    float dy = maxY - cy;
    radius = sqrt(dx * dx + dy * dy);
}

// Build bounding circle hierarchy using top-down splitting
Circle *buildBVH(const std::vector<int> &boxIndices, int depth = 0)
{
    if (boxIndices.empty())
        return nullptr;

    float cx, cy, radius;
    calculateBoundingCircle(boxIndices, cx, cy, radius);

    // Create leaf if we have 2 or fewer boxes or max depth
    if (boxIndices.size() <= 2 || depth >= 3)
    {
        Circle *leaf = new Circle(cx, cy, radius, true);
        leaf->boxIndices = boxIndices;
        return leaf;
    }

    // Split along the axis with larger extent
    float minX = 1e6, maxX = -1e6, minY = 1e6, maxY = -1e6;
    for (int idx : boxIndices)
    {
        minX = std::min(minX, boxes[idx].x);
        maxX = std::max(maxX, boxes[idx].x);
        minY = std::min(minY, boxes[idx].y);
        maxY = std::max(maxY, boxes[idx].y);
    }

    float extentX = maxX - minX;
    float extentY = maxY - minY;
    bool splitX = extentX > extentY;

    // Sort boxes along split axis
    std::vector<int> sortedIndices = boxIndices;
    if (splitX)
    {
        std::sort(sortedIndices.begin(), sortedIndices.end(),
                  [](int a, int b)
                  { return boxes[a].x < boxes[b].x; });
    }
    else
    {
        std::sort(sortedIndices.begin(), sortedIndices.end(),
                  [](int a, int b)
                  { return boxes[a].y < boxes[b].y; });
    }

    // Split in half
    int mid = sortedIndices.size() / 2;
    std::vector<int> leftIndices(sortedIndices.begin(), sortedIndices.begin() + mid);
    std::vector<int> rightIndices(sortedIndices.begin() + mid, sortedIndices.end());

    // Create internal node
    Circle *node = new Circle(cx, cy, radius, false);

    Circle *left = buildBVH(leftIndices, depth + 1);
    Circle *right = buildBVH(rightIndices, depth + 1);

    if (left)
        node->children.push_back(left);
    if (right)
        node->children.push_back(right);

    return node;
}

// Create 8 boxes along a sine curve and build BVH
void createGeometry()
{
    boxes.clear();

    // Create 8 boxes along a sine curve
    // Keep content centered in larger window
    float startX = 200;
    float endX = 600;
    float centerY = 300;
    float amplitude = 120;
    float frequency = 0.02f;
    float boxSize = 35.0f;

    for (int i = 0; i < 8; i++)
    {
        float t = (float)i / 7.0f;
        float x = startX + t * (endX - startX);
        float y = centerY + amplitude * sin(frequency * x);
        boxes.push_back(Box(x, y, boxSize));
    }

    // Build bounding volume hierarchy
    std::vector<int> allIndices;
    for (int i = 0; i < boxes.size(); i++)
    {
        allIndices.push_back(i);
    }

    root = buildBVH(allIndices);
}

// Display callback
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Calculate cursor box vertices
    float half = BOX_SIZE / 2.0f;
    float v1x = mouseX - half, v1y = mouseY - half;
    float v2x = mouseX + half, v2y = mouseY - half;
    float v3x = mouseX + half, v3y = mouseY + half;
    float v4x = mouseX - half, v4y = mouseY + half;

    // Get cursor bounding circle radius
    float cursorRadius = getCursorBoundingCircleRadius(v1x, v1y, v2x, v2y, v3x, v3y, v4x, v4y);

    // Reset collision states and perform collision detection
    resetCollisions(root);
    leafHit = false;
    collisionDetection(root, mouseX, mouseY, cursorRadius, leafHit);

    // Draw hierarchy
    drawHierarchy(root, leafHit);

    // Draw cursor box
    glColor3f(1.0f, 1.0f, 0.0f); // Yellow
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(v1x, v1y);
    glVertex2f(v2x, v2y);
    glVertex2f(v3x, v3y);
    glVertex2f(v4x, v4y);
    glEnd();

    // Draw cursor bounding circle
    glColor3f(1.0f, 1.0f, 0.0f);
    glLineWidth(1.0f);
    drawCircle(mouseX, mouseY, cursorRadius);

    // Draw status text
    glColor3f(1.0f, 1.0f, 1.0f);
    std::string status = "Collision State: ";
    if (leafHit)
    {
        status += "LEAF HIT - ALL RED";
    }
    else if (root->isColliding)
    {
        status += "INTERMEDIATE NODE HIT";
    }
    else
    {
        status += "NO COLLISION";
    }
    drawText(20, WINDOW_HEIGHT - 30, status);

    // Draw instructions
    glColor3f(0.7f, 0.7f, 0.7f);
    drawText(20, WINDOW_HEIGHT - 55, "Move mouse to test collisions");
    drawText(20, WINDOW_HEIGHT - 75, "Green = leaf nodes, Blue = internal nodes");

    glutSwapBuffers();
}

// Mouse motion callback
void motion(int x, int y)
{
    screenToGL(x, y, mouseX, mouseY);
    glutPostRedisplay();
}

// Passive motion callback (no button pressed)
void passiveMotion(int x, int y)
{
    screenToGL(x, y, mouseX, mouseY);
    glutPostRedisplay();
}

// Reshape callback
void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Initialize OpenGL
void init()
{
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    createGeometry();
    screenToGL(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, mouseX, mouseY);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Bounding Circle Hierarchy Visualization");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMotionFunc(motion);
    glutPassiveMotionFunc(passiveMotion);

    glutMainLoop();
    return 0;
}