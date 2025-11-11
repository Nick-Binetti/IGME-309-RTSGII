//Usage:
//Hold down the number keys , 1-7, to select one or multiple circles.
//While circle(s) are selected, use the left mouse button to translate and use the right mouse button to rotate.

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <iostream>
#include <math.h>
using namespace std;

#define MAX_NUM_PART 16

int win_width = 600, win_height = 600;
float canvas_width = 20.0f; float canvas_height = 20.0f;


bool keyStates[256];
int buttonState;
int selectNum = 0;

float colors[3 * MAX_NUM_PART];
float translations[2 * MAX_NUM_PART];
float rotations[MAX_NUM_PART];

float curMouse[2];
float preMouse[2];

void init(void)
{
	for (int i = 0; i < 256; i++) {
		keyStates[i] = false;
	}
	for (int i = 0; i < MAX_NUM_PART; i++) {
		colors[i * 3 + 0] = 0.0f; // red
		colors[i * 3 + 1] = 0.0f; // green
		colors[i * 3 + 2] = 0.0f; // blue

		translations[i * 2 + 0] = 0.0f; // x
		translations[i * 2 + 1] = 0.0f; // y

		rotations[i] = 0.0f;
	}

	buttonState = -1;
}

void drawRect(float w, float h, const float* c) {
	glColor3fv(c);
	glLineWidth(3.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-w / 2.0f, 0.0f);
	glVertex2f(w / 2.0f, 0.0f);
	glVertex2f(w / 2.0f, h);
	glVertex2f(-w / 2.0f, h);
	glEnd();
}

void display(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	int cid = -1; // the index of current part

	// --- torso ---
	cid = 0;
	glPushMatrix(); //torso start
	glTranslatef(translations[cid * 2 + 0], translations[cid * 2 + 1], 0.0f);
	glRotatef(rotations[cid], 0, 0, 1);
	drawRect(3.0f, 2.0f, colors + cid * 3);

	//initially select torso
	colors[(selectNum) * 3 + 0] = 1.0f;
	colors[(selectNum) * 3 + 1] = 0.0f;
	colors[(selectNum) * 3 + 2] = 0.0f;

		// --- chest ---
		cid = 1;
		glPushMatrix(); //chest start
		glTranslatef(0.0f, 2.0f, 0.0f); 
		glRotatef(rotations[cid], 0, 0, 1);
		drawRect(4.0f, 2.0f, colors + cid * 3);

			// --- neck --- 
			cid = 2;
			glPushMatrix(); //neck start
			glTranslatef(0.0f, 2.0f, 0.0f);
			glRotatef(rotations[cid], 0.0f, 0.0f, 1.0f);
			drawRect(0.5f, 1.0f, colors + cid * 3);

				// --- head ---
				cid = 3;
				glTranslatef(0.0f, 1.0f, 0.0f); 
				glRotatef(rotations[cid], 0, 0, 1);
				drawRect(2.0f, 2.0f, colors + cid * 3);
			
			glPopMatrix(); //end of neck

		// --- left arm ---
		cid = 4;
		glPushMatrix(); //left arm start
		glTranslatef(-2.5f, 1.5f, 0.0f); 
		glRotatef(rotations[cid], 0, 0, 1);
		drawRect(1.0f, 3.0f, colors + cid * 3);

			// --- left forearm ---
			cid = 5;
			glPushMatrix(); //left forearm start
			glTranslatef(0.0f, 3.0f, 0.0f); 
			glRotatef(rotations[cid], 0, 0, 1);
			drawRect(1.0f, 2.0f, colors + cid * 3);

				// --- left hand ---
				cid = 6;
				glPushMatrix(); //left hand start
				glTranslatef(0.0f, 2.0f, 0.0f); 
				glRotatef(rotations[cid], 0, 0, 1);
				drawRect(1.0f, 1.0f, colors + cid * 3);

				glPopMatrix(); //end of left hand

			glPopMatrix(); //end of left forearm

		glPopMatrix(); //end of left arm

		// --- right arm ---
		cid = 7;
		glPushMatrix(); //right arm start
		glTranslatef(2.5f, 1.5f, 0.0f);
		glRotatef(rotations[cid], 0, 0, 1);
		drawRect(1.0f, 3.0f, colors + cid * 3);

			// --- right forearm ---
			cid = 8;
			glPushMatrix(); //right forearm start
			glTranslatef(0.0f, 3.0f, 0.0f);
			glRotatef(rotations[cid], 0, 0, 1);
			drawRect(1.0f, 2.0f, colors + cid * 3);

				// --- right hand ---
				cid = 9;
				glPushMatrix(); //right hand start
				glTranslatef(0.0f, 2.0f, 0.0f);
				glRotatef(rotations[cid], 0, 0, 1);
				drawRect(1.0f, 1.0f, colors + cid * 3);

				glPopMatrix(); //end of right hand

			glPopMatrix(); //end of right forearm 

		glPopMatrix(); //end of right arm

		glPopMatrix(); //chest done

	// --- left leg ---
	cid = 10;
	glPushMatrix();
	glTranslatef(-1.0f, -2.0f, 0.0f); 
	glRotatef(rotations[cid], 0, 0, 1);
	drawRect(1.0f, 3.0f, colors + cid * 3);

		// --- left shin ---
		cid = 11;
		glPushMatrix();
		glTranslatef(0.0f, -3.0f, 0.0f); 
		glRotatef(rotations[cid], 0, 0, 1);
		drawRect(0.8f, 3.0f, colors + cid * 3);

			// --- left foot ---
			cid = 12;
			glPushMatrix();
			glTranslatef(-0.5f, -1.0f, 0.0f);
			glRotatef(rotations[cid], 0, 0, 1);
			drawRect(2.0f, 1.0f, colors + cid * 3);

			glPopMatrix(); //end of left foot

		glPopMatrix(); //end of left shin

	glPopMatrix(); //end of left leg

	// --- right leg ---
	cid = 13;
	glPushMatrix();
	glTranslatef(1.0f, -2.0f, 0.0f);
	glRotatef(rotations[cid], 0, 0, 1);
	drawRect(1.0f, 3.0f, colors + cid * 3);

		// --- right shin ---
		cid = 14;
		glPushMatrix();
		glTranslatef(0.0f, -3.0f, 0.0f);
		glRotatef(rotations[cid], 0, 0, 1);
		drawRect(0.8f, 3.0f, colors + cid * 3);

			// --- right foot ---
			cid = 15;
			glPushMatrix();
			glTranslatef(0.5f, -1.0f, 0.0f);
			glRotatef(rotations[cid], 0, 0, 1);
			drawRect(2.0f, 1.0f, colors + cid * 3);

			glPopMatrix(); //end of right foot

		glPopMatrix(); //end of right shin

	glPopMatrix(); //end of right leg

	glPopMatrix(); //end of torso 

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

void specialKeyboard(int key, int x, int y)
{

	if (key == GLUT_KEY_DOWN)
	{
		selectNum++;
		if (selectNum > 15)
		{
			//deselects the right foot before looping back to the torso
			colors[(15) * 3 + 0] = 0.0f;
			colors[(15) * 3 + 1] = 0.0f;
			colors[(15) * 3 + 2] = 0.0f;
			selectNum = 0;
		}
		colors[(selectNum) * 3 + 0] = 1.0f;
		colors[(selectNum) * 3 + 1] = 0.0f;
		colors[(selectNum) * 3 + 2] = 0.0f;
		//unselect last part 
		colors[(selectNum - 1) * 3 + 0] = 0.0f;
		colors[(selectNum - 1) * 3 + 1] = 0.0f;
		colors[(selectNum - 1) * 3 + 2] = 0.0f;
	}

	if (key == GLUT_KEY_UP)
	{
		selectNum--;
		if (selectNum <= -1)
		{
			//deselects the torso before looping back to the right foot
			colors[(0) * 3 + 0] = 0.0f;
			colors[(0) * 3 + 1] = 0.0f;
			colors[(0) * 3 + 2] = 0.0f;
			selectNum = 15;
		}
		colors[(selectNum) * 3 + 0] = 1.0f;
		colors[(selectNum) * 3 + 1] = 0.0f;
		colors[(selectNum) * 3 + 2] = 0.0f;
		//unselect last part 
		colors[(selectNum + 1) * 3 + 0] = 0.0f;
		colors[(selectNum + 1) * 3 + 1] = 0.0f;
		colors[(selectNum + 1) * 3 + 2] = 0.0f;
	}
	glutPostRedisplay();

}
void keyboard(unsigned char key, int x, int y)
{
	if (key == 27) // 'esc' key
		exit(0);

	if (key == 97)
	{
		rotations[selectNum] += 1.0f;
	}

	if (key == 100)
	{
		rotations[selectNum] -= 1.0f;
	}

	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) {
		buttonState = button;
		curMouse[0] = ((float)x / win_width - 0.5f) * canvas_width;
		curMouse[1] = ((float)(win_height - y) - 0.5f) / win_height * canvas_height;
		preMouse[0] = ((float)x / win_width - 0.5f) * canvas_width;
		preMouse[1] = ((float)(win_height - y) - 0.5f) / win_height * canvas_height;
	}
	else if (state == GLUT_UP)
		button = -1;
}

void motion(int x, int y)
{
	unsigned char asciiOffset = 49; // see an ascii table

	curMouse[0] = ((float)x / win_width - 0.5f) * canvas_width;
	curMouse[1] = ((float)(win_height - y) - 0.5f) / win_height * canvas_height;
	if (buttonState == GLUT_LEFT_BUTTON) {
		translations[(selectNum) * 2 + 0] += curMouse[0] - preMouse[0];
		translations[(selectNum) * 2 + 1] += curMouse[1] - preMouse[1];
		glutPostRedisplay();
	}

	else if (buttonState == GLUT_RIGHT_BUTTON) {
		rotations[selectNum] += curMouse[0] - preMouse[0];
		glutPostRedisplay();
	}

	preMouse[0] = curMouse[0];
	preMouse[1] = curMouse[1];

}

int main(int argc, char* argv[])
{
	init();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(win_width, win_height);
	glutCreateWindow("2D Transformation Tree");

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutSpecialFunc(specialKeyboard);

	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutMainLoop();
	return 0;
}
