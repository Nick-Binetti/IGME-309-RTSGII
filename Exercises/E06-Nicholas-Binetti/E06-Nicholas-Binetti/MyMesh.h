#pragma once

#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

class MyMesh
{
private:
	unsigned int vertNum;
	unsigned int triNum;

	float *vertices;	   // size: vertNum * 2 (x,y)
	unsigned int *indices; // size: triNum * 3
	float *vertColors;	   // size: vertNum * 3 (r,g,b)

	// GPU buffers
	GLuint vboVerts = 0;  // GL_ARRAY_BUFFER for positions
	GLuint vboColors = 0; // GL_ARRAY_BUFFER for colors
	GLuint iboElems = 0;  // GL_ELEMENT_ARRAY_BUFFER for indices

	void createOrUpdateGPU(); // upload CPU arrays to GPU buffers

public:
	MyMesh();
	~MyMesh();

	void load(char *fileName);
	void load(const unsigned int p_vertNum,
			  const unsigned int p_triNum,
			  const float *p_vertices,
			  const unsigned int *p_indices);

	void update(); // no-op for now
	void draw();   // VBO + IBO draw

	unsigned int getVertNum() { return vertNum; }
	unsigned int getTriNum() { return triNum; }
};
