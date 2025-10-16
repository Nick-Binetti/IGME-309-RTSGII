#include <GL/glew.h>
#include <GL/wglew.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include "Camera.h"
#include "Text.h"
#include "ParticleSystem.h"

#include <iostream>
using namespace std;

int g_winWidth = 1024;
int g_winHeight = 512;

Camera g_cam;
Text g_text;
unsigned char g_keyStates[256];

/**************************************************/

// Particle system object
// You can pass different numbers of particles into the constructor of the particle system object
ParticleSystem g_particles = ParticleSystem(5000);

/*************************************************/


// tracking the game time - millisecond 1)
unsigned int curTime = 0;
unsigned int preTime = 0;
float deltaTime = 0.0f;
float elapsedTime = 0.0f;
int frameCount = 0;
int fps = 0;

void init(void)
{
	// init the camera
	g_cam.set(30.0f, 40.0f, 30.0f, 0.0f, 0.0f, 0.0f, g_winWidth, g_winHeight);
	// set the color of on-screen texts
	g_text.setColor(0.0f, 0.0f, 0.0f);
}

void initialGL()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	//glCullFace(GL_BACK);
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// specify polygon mode for rendering
	glPolygonMode(GL_FRONT, GL_FILL);

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void idle()
{
	curTime = glutGet(GLUT_ELAPSED_TIME); // returns the number of milliseconds since glutInit() was called.
	deltaTime = (float)(curTime - preTime) / 1000.0f; // frame-different time in seconds 

	// update fps
	frameCount++;
	elapsedTime += deltaTime;
	if (elapsedTime >= 1.0f) {
		elapsedTime = 0.0f;
		fps = frameCount;
		frameCount = 0;
	}

	// update particle system
	g_particles.update(deltaTime);

	preTime = curTime; // the curTime become the preTime for the next frame

	// update camera
	g_cam.keyOperation(g_keyStates, g_winWidth, g_winHeight);

	glutPostRedisplay();
}

void display()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// add things you'd like to draw ....
	glMatrixMode(GL_MODELVIEW);
	g_particles.draw();

	g_cam.drawGrid();

	// display the text
	if (g_cam.isFocusMode()) {
		string str = "Cam mode: Focus";
		g_text.draw(10, 30, const_cast<char*>(str.c_str()), g_winWidth, g_winHeight);
	}
	else if (g_cam.isFPMode()) {
		string str = "Cam mode: FP";
		g_text.draw(10, 30, const_cast<char*>(str.c_str()), g_winWidth, g_winHeight);
	}

	char s1[128];
	char s2[128];
	sprintf_s(s1, "Number of Particles: %d", g_particles.getNumParticles());
	sprintf_s(s2, "FPS: %d", fps);
	g_text.draw(10, 50, s1, g_winWidth, g_winHeight);
	g_text.draw(10, 70, s2, g_winWidth, g_winHeight);


	glutSwapBuffers();
}

void reshape(int w, int h)
{
	g_winWidth = w;
	g_winHeight = h;
	if (h == 0) {
		h = 1;
	}
	g_cam.setProj(g_winWidth, g_winHeight);
	g_cam.setModelView();
	glViewport(0, 0, w, h);
}

void mouse(int button, int state, int x, int y)
{
	g_cam.mouseClick(button, state, x, y, g_winWidth, g_winHeight);

}

void motion(int x, int y)
{
	g_cam.mouseMotion(x, y, g_winWidth, g_winHeight);
}

void keyup(unsigned char key, int x, int y)
{
	g_keyStates[key] = false;
}

void keyboard(unsigned char key, int x, int y)
{
	g_keyStates[key] = true;
	switch (key) {
	case 27:
		exit(0);
		break;
	case 'c': // switch cam control mode
		g_cam.switchCamMode();
		glutPostRedisplay();
		break;
	case ' ':
		g_cam.PrintProperty();
		break;
	}
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(g_winWidth, g_winHeight);
	glutCreateWindow("Particle System");

	glewInit();
	initialGL();
	init();

	// Disable V-Sync, which can otherwise cap the FPS at the monitor's maximum refresh rate.
	if (wglewIsSupported("WGL_EXT_swap_control")) {
		wglSwapIntervalEXT(0); 
	}

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardUpFunc(keyup);
	glutKeyboardFunc(keyboard);
	
	preTime = curTime = glutGet(GLUT_ELAPSED_TIME);

	glutMainLoop();
	return 0;
}