#include "PolyObject.h"

/// <summary>
/// Creates black polygon object
/// </summary>
PolyObject::PolyObject()
{
	color = { 0.0f, 0.0f, 0.0f };
}

/// <summary>
/// Adds x and y vertices to vector
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
void PolyObject::addVertex(float x, float y)
{
	vertices.push_back(x);
	vertices.push_back(y);
}

/// <summary>
/// Sets color of current polygon
/// </summary>
/// <param name="cr"></param>
/// <param name="cg"></param>
/// <param name="cb"></param>
void PolyObject::setColor(float cr, float cg, float cb)
{
    color.clear();
	color.push_back(cr);
	color.push_back(cg);
	color.push_back(cb);
}
/// <summary>
/// Returns the number of vertex pairs
/// </summary>
/// <returns></returns>
unsigned int PolyObject::getVertNum() const
{
	return vertices.size()/2;
}

/// <summary>
/// Meant for draw completed polygons
/// </summary>
void PolyObject::draw() const
{
    unsigned int count = getVertNum();
    if (count == 0) return;

    glColor3f(color[0], color[1], color[2]);

    if (count == 1)
    {
        glBegin(GL_POINTS);
        glVertex2f(vertices[0], vertices[1]);
        glEnd();
    }
    else if (count == 2)
    {
        glBegin(GL_LINES);
        glVertex2f(vertices[0], vertices[1]);
        glVertex2f(vertices[2], vertices[3]);
        glEnd();
    }
    else
    {
        glBegin(GL_POLYGON);
        for (size_t i = 0; i < vertices.size(); i += 2)
            glVertex2f(vertices[i], vertices[i + 1]);
        glEnd();
    }
}
/// <summary>
/// Meant for drawing incomplete polygons still being populated
/// </summary>
/// <param name="mouseX"></param>
/// <param name="mouseY"></param>
void PolyObject::draw(float mouseX, float mouseY)
{
    unsigned int count = getVertNum();
    if (count == 0) return;

    glColor3f(color[0], color[1], color[2]);

    if (count == 1)
    {
        glBegin(GL_LINES);
        glVertex2f(vertices[0], vertices[1]);
        glVertex2f(mouseX, mouseY);
        glEnd();
    }
    else if (count >= 2)
    {
        glBegin(GL_POLYGON);
        for (size_t i = 0; i < vertices.size(); i += 2)
            glVertex2f(vertices[i], vertices[i + 1]);
        glVertex2f(mouseX, mouseY);
        glEnd();
    }
}

