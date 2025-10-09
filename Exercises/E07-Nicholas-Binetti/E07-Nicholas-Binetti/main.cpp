#include <GL/glew.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

// include the gl mathematic library 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/constants.hpp>

#include "MyMesh.h"

using namespace std;
using namespace glm;

int g_winWidth = 1024;
int g_winHeight = 512;

// global parameters for defining the light and shading effects
GLfloat light0_pos[] = { 0.0f, 5.0f, 5.0f, 0.0f };
GLfloat light0_Amb[] = { 0.4f, 0.3f, 0.3f, 1.0f };
GLfloat light0_Diff[] = { 0.8f, 0.8f, 0.7f, 1.0f };
GLfloat light0_Spec[] = { 0.9f, 0.9f, 0.9f, 1.0f };


unsigned char g_keyStates[256];
MyMesh g_mesh = MyMesh();

// an object file path - a bunny model with 2K vertices
const char meshFile[128] = "Mesh/bunny2k.obj";

float theta = 0.0f;
float cam_x = 5.0f;
float cam_y = 5.0f;
float cam_z = 0.0f;
float radius = 5.0f;

int curTime;
int preTime;

void reshape(int w, int h);

void init(void)
{
	// loading the input mesh
	g_mesh.loadFromOBJ(meshFile);
}

void initialGL()
{
	// init GL light settings
	glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_Amb);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_Diff);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_Spec);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// specify polygon mode for rendering
	glPolygonMode(GL_FRONT, GL_FILL);


	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glShadeModel(GL_SMOOTH);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void idle()
{
	curTime = glutGet(GLUT_ELAPSED_TIME);
	float deltaTime = (float)(curTime - preTime) / 1000.0f;

	/*****************************/
	// Write your code below
	//calculate theta
	theta += 1 * deltaTime;

	//set camera's x and z positions
	cam_x = radius*cos(theta);
	cam_z = radius*sin(theta);

	//update camera view
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	gluLookAt(cam_x, cam_y, cam_z, 0, 0, 0, 0, 1, 0);
	
	// Write your code above
	/*****************************/

	preTime = curTime;
	glutPostRedisplay();
}

void display()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// use the specified lighting and shading properties to render the scene
	// this will bypass the default flat shading mode
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);

	g_mesh.draw();
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	g_winWidth = w;
	g_winHeight = h;
	if (h == 0) {
		h = 1;
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cam_x, cam_y, cam_z, 0, 0, 0, 0, 1, 0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, w / h, 0.1, 1000);

	glViewport(0, 0, w, h);
}


int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(g_winWidth, g_winHeight);
	glutCreateWindow("Orbiting Camera");

	glewInit();
	initialGL();
	init();

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(idle);

	preTime = glutGet(GLUT_ELAPSED_TIME);
	glutMainLoop();
	return 0;


}
