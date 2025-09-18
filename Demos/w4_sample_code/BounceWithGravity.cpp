#ifdef __APPLE__
#include <GLUT/glut.h> // include glut for Mac
#else
#include <GL/freeglut.h> //include glut for Windows
#endif

#include <math.h>
#include <iostream>

using namespace std;

// the window's width and height
int width, height;

// global parameters defining the circle
int vertNum = 100; // total number of vertices for the circle
float x = 0.0f, y = 1.5f; // center postion of the circle
float r = 0.2f; // circle's radius
float vel = -2.2f; // the velocity of the ball 
float acc = -9.8f; // acceleration of gravity 

// define the y value of the ground
float floor_y = -3.0f;

// tracking the game time - millisecond 
unsigned int curTime = 0;
unsigned int preTime = 0;

// algo. for a falling ball: v = v' + g*t; where g is acceleration of the gavity equal to -9.8, t is delta time between frames

void init(void)
{
	// initialize the size of the window
	width = 600;
	height = 600;
}

void display(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();



	// draw a circle as the ball
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < vertNum; ++i) {
		float t = (float)i / vertNum * 2.0f * 3.14f;
		glVertex2f(x + r * cos(t), y + r * sin(t));
	}
	glEnd();

	// use GL_LINES to draw the floor 
	glLineWidth(3); // define the width of the line
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINES);
	glVertex2f(-2.0f, floor_y);
	glVertex2f(2.0f, floor_y);
	glEnd();
	glLineWidth(1);

	glutSwapBuffers();
}


void update()
{
	curTime = glutGet(GLUT_ELAPSED_TIME); // returns the number of milliseconds since glutInit() was called.
	float deltaTime = (float)(curTime - preTime) / 1000.0f; // frame-different time in seconds 

	// if the ball doesn't hit the floor, its vel should be updated based on gravity
	if (y - r > floor_y) {
		vel += acc * deltaTime;
	}

	// if the ball hits the floor, the ball should be bounced up, so the vel should be positive to make the ball move upward
	else {
		vel = abs(vel);
	}

	// update the position of the ball
	// since this example only allows the ball moves along y axis, we only need to update the y value
	y += vel * deltaTime;


	preTime = curTime; // make the curTime become the preTime for the next frame
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	width = w;
	height = h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-5.0, 5.0, -5.0, 5.0);
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	if (key == 27)
		exit(0);
}


int main(int argc, char* argv[])
{
	init();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize((int)width, (int)height);
	glutCreateWindow("Ball Bounces Based on Gravity");

	/* --- register callbacks with GLUT --- */
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(update);

	//start the glut main loop
	glutMainLoop();

	return 0;
}
