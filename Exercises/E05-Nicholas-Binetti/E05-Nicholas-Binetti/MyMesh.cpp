#include "MyMesh.h"
#include <GL/freeglut.h>

#include <iostream>
#include <string>
#include <fstream>
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
}

void MyMesh::load(char* fileName)
{
	vertNum = 0;
	triNum = 0;

	vertices = new float[100 * 2];
	indices = new unsigned int[100 * 3];
	vertColors = new float[100 * 3];

	ifstream file(fileName); //opens file
	string line;

	if (!file.is_open())
	{
		/****************************************/
		// Write your code below
		cout << "OBJ File not found :(";
		return;
		// Write your code above
		/****************************************/
	}

	/****************************************/
	// Write your code below
	int lineNum = 0;
	int faceNum = 0;
	//separates each line of the obj file
	while (getline(file, line))
	{
		//if vertex
		if (line[0] == 'v')
		{
			int pos = 1;
			while (pos != line.length())
			{
				//single vertex
				string vert;

				//position of first vertex decimal
				int decPos = line.find(".", pos + 1);

				//gets numbers before decimal
				for (int i = pos + 1; i < decPos; i++)
				{
					vert += line[i];
				}

				//position of space after first vertex
				int spacePos;
				if (line.find(" ", decPos) < 18446744073709551615) //if no spaces found line.find = int.max
				{
					spacePos = line.find(" ", decPos);
				}
				else
				{
					spacePos = line.length();
					//gets numbers after decimal
					for (int i = decPos; i < spacePos; i++)
					{
						vert += line[i];
					}
					//reassign pos to start before next vertex
					pos = spacePos;
					vertices[vertNum] = stof(vert);
					//cout << vertices[vertNum] << endl;

					vertNum++;
					break;
				}

				//gets numbers after decimal
				for (int i = decPos; i < spacePos; i++)
				{
					vert += line[i];
				}
				//reassign pos to start before next vertex
				pos = spacePos;
				vertices[vertNum] = stof(vert);
				//cout << vertices[vertNum] << endl;
				vertNum++;
			}
		}

		//if face
		if (line[0] == 'f')
		{
			int pos = 1;
			while (pos != line.length())
			{
				string face;

				int spacePos = line.find(" ", pos + 1);

				if (spacePos == -1)
				{
					spacePos = line.length();
				}

				for (int i = pos + 1; i < spacePos; i++)
				{
					face += line[i];
				}
				pos = spacePos;

				indices[faceNum] = stof(face) -1 ;
				//cout << indices[faceNum] << endl;
				faceNum++;
			}
			triNum++;
		}
	}

	// Write your code above
	/****************************************/

	// generate random vertex colors
	for (unsigned int i = 0; i < vertNum; i++)
	{
		vertColors[i * 3 + 0] = float(rand()) / (RAND_MAX + 1);
		vertColors[i * 3 + 1] = float(rand()) / (RAND_MAX + 1);
		vertColors[i * 3 + 2] = float(rand()) / (RAND_MAX + 1);
	}
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
	for (unsigned int i = 0; i < vertNum; i++)
	{
		vertColors[i * 3 + 0] = float(rand()) / (RAND_MAX + 1);
		vertColors[i * 3 + 1] = float(rand()) / (RAND_MAX + 1);
		vertColors[i * 3 + 2] = float(rand()) / (RAND_MAX + 1);
	}
}

void MyMesh::update()
{
	// we will use this for transformations later....
}

void MyMesh::draw()
{
	for (int i = 0; i < triNum; ++i)
	{
		glBegin(GL_TRIANGLES);
		glColor3fv(vertColors + indices[i * 3 + 0] * 3);
		glVertex2fv(vertices + indices[i * 3 + 0] * 2);

		glColor3fv(vertColors + indices[i * 3 + 1] * 3);
		glVertex2fv(vertices + indices[i * 3 + 1] * 2);

		glColor3fv(vertColors + indices[i * 3 + 2] * 3);
		glVertex2fv(vertices + indices[i * 3 + 2] * 2);
		glEnd();
	}
}