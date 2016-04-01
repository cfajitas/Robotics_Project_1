#ifndef INTERFACE_H
#define INTEFACE_H

#include <stdlib.h>
#include <GL/glut.h>
#include <cstdarg>

#include "MouseAction.h"
#include "Button.h"
#include "Robot.h"
#include "Graphics.h"

#include "Server.h"
#include "Client.h"

using namespace std;

extern int MODE;
extern char IPADDRESS[1024];

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600
#define WINDOW_START_X 800
#define WINDOW_START_Y 100


class Interface {
private:
	static Interface* s_instance;
	Interface(int argc, char** argv);
public:
	static Interface* getInstance();
	static Interface* getInstance(int argc, char** argv);

	//DWORD WINAPI RunServer(LPVOID lpParameter);

	// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~
	// =~=~=~  VARIABLES  ~=~=~=~=~=~=~=~
	// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~

	Robot* robot;

	MouseAction* mouseAction;
	vector<Button*> buttons;
	Graphics* graphics;

	Server* server;
	Client* client;

	bool simulate_delay;

	// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~
	// =~=~=~  CONSTRUCTORS  =~=~=~=~=~=~
	// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~

	// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~
	// =~=~=~  FUNCTIONS  ~=~=~=~=~=~=~=~
	// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~

	void update();
	void drawUpdate();

	void determineAction();

	Button* decodeMouse();

	void processServer();

	// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~
	// =~=~  Robot Functions  ~=~=~=~=~=~
	// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~

	// Callback functions have to be static
	static void paintAction();

	// Joint controls
	static void joint0SlideLeft();
	static void joint0SlideRight();

	static void joint1CCW();
	static void joint1CW();

	static void joint2CCW();
	static void joint2CW();

	// World controls
	static void paintBrushSlideLeft();
	static void paintBrushSlideRight();
	static void paintBrushSlideUp();
	static void paintBrushSlideDown();

	// Settings
	static void increaseStep();
	static void decreaseStep();


	static void serverFinish();

};

#endif