// Robotics_PRR_Robot.cpp : Defines the entry point for the console application.

// <stdlib.h> ALWAYS HAS TO BE INCLUDED BEFORE <GL/glut.h> OR THINGS BREAAAAAAAAAAAAK
#include <stdlib.h>
#include <GL/glut.h>

#include "Interface.h"

using namespace std;

int MODE = -1;
char IPADDRESS[1024];

//Interface* gui;

/*
	button = left/middle/right
	state = GLUT_UP / GLUT_DOWN
	x/y = pixels
*/
void mouseButton(int button, int state, int x, int y) {
	if (state != GLUT_UP) return;	// Only proceeds when the mouse button is released
									// Changes to the screen only happen when the mouse button is released (a button may be pressed)
	Interface::getInstance()->mouseAction = new MouseAction(x, y, button, state);
	glutPostRedisplay();	// Calls render()
							// If this was at the end of render(), the program would loop foreverrrrr
}

void keyboard(unsigned char key, int x, int y) {

	if (key == ' ') {
		Interface::getInstance()->paintAction();
		glutPostRedisplay();
	}

}

void arrowPresses(int key, int x, int y) {
	switch (key) {

	case GLUT_KEY_UP:
		Interface::getInstance()->paintBrushSlideUp();
		break;
	case GLUT_KEY_DOWN:
		Interface::getInstance()->paintBrushSlideDown();
		break;
	case GLUT_KEY_LEFT:
		Interface::getInstance()->paintBrushSlideLeft();
		break;
	case GLUT_KEY_RIGHT:
		Interface::getInstance()->paintBrushSlideRight();
		break;
	}

	glutPostRedisplay();
}

void render() {
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);	// Clears to black
	//---------------------------

	Interface::getInstance()->update();
	Interface::getInstance()->drawUpdate();

	//---------------------------
	glutSwapBuffers();
}

int main(int argc, char** argv) {

	char in;
	while (MODE == -1)
	{
		printf("Please Enter s for Server or c for Client \n");
		in = getc(stdin);
		cin.ignore(512, '\n');
		if (in == 's') {
			MODE = 0;
			printf("Entering Server Mode \n");
		}
		else if (in == 'c') {
			MODE = 1;
			printf("Entering Client Mode\n");
		}
		else {
			printf("ERROR Enter Valid Input \n");
		}
	}

	if (MODE == 1) {
		printf("Please Enter IP Address\n");
		scanf_s("%s", IPADDRESS, 1024);
	}

	//Set ip here for easy testing
	//IPADDRESS = "10.0.1.59";

	Interface::getInstance(argc, argv);

	glutDisplayFunc(render);
	glutMouseFunc(mouseButton);
	glutSpecialFunc(arrowPresses);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 1;
}

