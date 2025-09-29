
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include "MyMesh.h"

#include <iostream>
using namespace std;

float canvasSize[] = { 10.0f, 10.0f };
int rasterSize[] = { 600, 600 };

MyMesh g_mesh = MyMesh();


void init(void)
{
	// load the mesh data to g_mesh
	char fileName[10] = { "data2.obj" };

	g_mesh.load(fileName);
}


void display(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// draw the mesh
	g_mesh.draw();

	glutSwapBuffers();
}

void reshape(int w, int h)
{
	rasterSize[0] = w;
	rasterSize[1] = h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0f, canvasSize[0], 0.0f, canvasSize[1]);
	glViewport(0, 0, rasterSize[0], rasterSize[1]);

	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		exit(0);
		break;
	}
}

int main(int argc, char* argv[])
{
	init();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(rasterSize[0], rasterSize[1]);
	glutCreateWindow("MyMesh class example");

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;


}