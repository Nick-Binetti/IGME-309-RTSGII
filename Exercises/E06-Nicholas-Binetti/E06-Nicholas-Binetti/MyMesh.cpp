#include "MyMesh.h"

#include <iostream>
#include <time.h>
using namespace std;

MyMesh::MyMesh()
{
	vertNum = triNum = 0;
	vertices = NULL;
	indices = NULL;
	vertColors = NULL;
}

MyMesh::~MyMesh()
{
	if (vertices != NULL)
		delete[] vertices;
	if (indices != NULL)
		delete[] indices;
	if (vertColors != NULL)
		delete[] vertColors;

	// ********************************************************
	// Write code below
	// Delete GPU buffers
	if (vboVerts)
	{
		glDeleteBuffers(1, &vboVerts);
	}
	if (vboColors)
	{
		glDeleteBuffers(1, &vboColors);
	}
	if (iboElems)
	{
		glDeleteBuffers(1, &iboElems);
	}


	// Write code above
	// ********************************************************
}

void MyMesh::load(char* fileName)
{
}

void MyMesh::load(const unsigned int p_vertNum, const unsigned int p_triNum, const float* p_vertices, const unsigned int* p_indices)
{
	vertNum = p_vertNum;
	triNum = p_triNum;

	vertices = new float[vertNum * 2];
	indices = new unsigned int[triNum * 3];
	vertColors = new float[vertNum * 3];

	for (unsigned int i = 0; i < vertNum * 2; i++)
	{
		vertices[i] = p_vertices[i];
	}

	for (unsigned int i = 0; i < triNum * 3; i++)
	{
		indices[i] = p_indices[i];
	}

	// generate random vertex colors

	time_t t;
	srand((unsigned)time(&t));
	for (unsigned int i = 0; i < vertNum; i++)
	{
		vertColors[i * 3 + 0] = float(rand()) / (RAND_MAX + 1);
		vertColors[i * 3 + 1] = float(rand()) / (RAND_MAX + 1);
		vertColors[i * 3 + 2] = float(rand()) / (RAND_MAX + 1);
	}

	// Start creating GPU data.
	createOrUpdateGPU();
}

void MyMesh::createOrUpdateGPU()
{
	// ********************************************************
	// Write code below

	// Create buffers
	
	//vertices
	if (vboVerts == 0)
	{
		glGenBuffers(1, &vboVerts);
	}
	//colors
	if (vboColors == 0)
	{
		glGenBuffers(1, &vboColors);
	}
	//indices
	if (iboElems == 0)
	{
		glGenBuffers(1, &iboElems);
	}

	// Bind & copy Vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vboVerts);
	glBufferData(GL_ARRAY_BUFFER, vertNum * 2 * sizeof(float), vertices, GL_STATIC_DRAW);

	// Bind & copy Color buffer
	glBindBuffer(GL_ARRAY_BUFFER, vboColors);
	glBufferData(GL_ARRAY_BUFFER, vertNum * 3 * sizeof(float), vertColors, GL_STATIC_DRAW);

	// Bind & copy Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboElems);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, triNum * 3 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	// Unbind buffers for safety
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Write code above
	// ********************************************************

}



void MyMesh::update()
{
	// we will use this for transformations later....
}

void MyMesh::draw()
{
	// Enable fixed-function client states
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	// ********************************************************
	// Write code below
	// Draw shape with buffer data
	// 
	 // Bind vertex buffer and set pointer
	glBindBuffer(GL_ARRAY_BUFFER, vboVerts);
	glVertexPointer(2, GL_FLOAT, 0, (void*)0);

	// Bind color buffer and set pointer
	glBindBuffer(GL_ARRAY_BUFFER, vboColors);
	glColorPointer(3, GL_FLOAT, 0, (void*)0);

	// Bind index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboElems);

	// Draw using index buffer
	glDrawElements(GL_TRIANGLES, triNum * 3, GL_UNSIGNED_INT, (void*)0);

	// Unbind buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Write code above
	// ********************************************************

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}