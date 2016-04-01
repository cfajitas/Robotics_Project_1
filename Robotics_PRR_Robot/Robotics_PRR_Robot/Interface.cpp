#include "Interface.h"
#include <queue>
#include <Windows.h>

//store thread data
struct thread_data
{
	int m_id;
	thread_data(int id) : m_id(id) {}
};

Server* global_server;


Interface* Interface::s_instance;
Interface* Interface::getInstance() {
	return s_instance;
}
Interface* Interface::getInstance(int argc, char** argv) {
	if (s_instance == NULL) {
		s_instance = new Interface(argc, argv);
	}
	return s_instance;
}

//Server initial thread
static DWORD WINAPI RunServerThread(LPVOID lpParameter) {
	global_server->ServerRun();
	return 0;
}

//Sever gui initial thread
static DWORD WINAPI RunGUIThread(LPVOID lpParameter) {
	Interface::getInstance()->serverFinish();
	return 0;
}

Interface::Interface(int argc, char** argv) {

	const char* window_name = "Project 1";

	robot = new Robot();
	graphics = new Graphics(argc, argv, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_START_X, WINDOW_START_Y, window_name);


	if (MODE == 0) {
		server = new Server(NULL,"50000", serverFinish, robot);
		global_server = server;
		simulate_delay = false; //change variable according to button instead

		//run threads (server side only)
		CreateThread(NULL, 0, RunServerThread, new thread_data(0), 0, 0);
		CreateThread(NULL, 0, RunGUIThread, new thread_data(1), 0, 0);
	}

	else {
		client = new Client(IPADDRESS,"50000");
	}

	// For initialization: 0, 0, 1, 1
		// means column 0, row 0, normal width, normal height
	// 3, 0.5f, 1, 1
		// means column 3, row 0.5, normal width, normal height
	// This initialization is then transformed into proper screen position in 'positionButtons()'

	// Joint buttons
		// Slider
		buttons.push_back(new Button("Slide left", joint0SlideLeft));
		buttons.push_back(new Button("Slide right",joint0SlideRight));
		// Joint 1
		buttons.push_back(new Button("CCW1", joint1CCW));
		buttons.push_back(new Button("CW1", joint1CW));
		// Joint 2
		buttons.push_back(new Button("CCW2", joint2CCW));
		buttons.push_back(new Button("CW2", joint2CW));

	// Paint
	buttons.push_back(new Button("Paint", paintAction));

	// World buttons
		// Left
		buttons.push_back(new Button("Left", paintBrushSlideLeft));
		// Up/Down
		buttons.push_back(new Button("Up", paintBrushSlideUp));
		buttons.push_back(new Button("Down", paintBrushSlideDown));
		// Right
		buttons.push_back(new Button("Right", paintBrushSlideRight));

	// Settings
		buttons.push_back(new Button("+", increaseStep));
		buttons.push_back(new Button("-", decreaseStep));

		graphics->positionButtons(buttons);

}


Button* Interface::decodeMouse() {

	// For the current 'mouseAction', find out if a button has been pressed

	if (mouseAction == NULL) return NULL;
	if (mouseAction->state != GLUT_UP) return NULL;

	int x = mouseAction->x;
	int y = SCREEN_HEIGHT -mouseAction->y;

	for (auto &button : buttons) {
		float x0 = button->x;
		float x1 = button->x + button->w;
		float y0 = button->y;
		float y1 = button->y + button->h;

		x0 = ((x0 + 1) / 2.0f)*SCREEN_WIDTH;
		x1 = ((x1 + 1) / 2.0f)*SCREEN_WIDTH;
		y0 = ((y0 + 1) / 2.0f)*SCREEN_HEIGHT;
		y1 = ((y1 + 1) / 2.0f)*SCREEN_HEIGHT;

		if (x0 <= x && x <= x1 && y0 <= y && y <= y1) {
			mouseAction = NULL;
			return button;
		}
	}

	return NULL;
}

void Interface::determineAction() {
	//  Based on the current MouseAction, apply changes to robot
	Button* targetButton = decodeMouse();

	if (MODE == 0) {
		processServer();
	}
	
	if (targetButton != NULL) {
		if (MODE == 1) {
			printf("Button: %s\n", targetButton->getLabel());
			client->ClientSend(targetButton->getLabel());
		}
		targetButton->onClick();
	}
}

void Interface::processServer() {
	
	char* buttonName;
	Button* target = NULL;

	//needs fixing 
	/*
	while (server->isServerDone())
	{
		buttonName = server->ServerRun();
		printf("Recieved: %s\n", buttonName);

		for (auto &button : buttons) {
			if (button->getLabel() == buttonName) {
				target = button;
			}
		}
		target->onClick();
	}
	*/
}



void Interface::update() {

	// Redraw the screen
	graphics->drawBackground();
	graphics->drawButtons(buttons);
	
	// Respond to user input
	determineAction();
}

void Interface::drawUpdate() {
	// Since the robot could be in a different position or orientation, we need to update these changes to the screen
	float data[8] = { robot->joint0x, robot->joint0y, robot->joint1x, robot->joint1y, robot->joint2x, robot->joint2y, robot->paintx, robot->painty };

	graphics->drawRobot(data);
	graphics->drawPaint();
}

void Interface::paintAction() {
	Interface::getInstance()->graphics->addPaint(Interface::getInstance()->robot->paintx, Interface::getInstance()->robot->painty);
}

void Interface::joint0SlideLeft() {
	Interface::getInstance()->robot->joint0Adjust( -1 * Interface::getInstance()->robot->joint_slide_amount);
}

void Interface::joint0SlideRight() {
	Interface::getInstance()->robot->joint0Adjust(Interface::getInstance()->robot->joint_slide_amount);
}

void Interface::joint1CCW() {
	Interface::getInstance()->robot->joint1Adjust(Interface::getInstance()->robot->joint_rotate_amount);
}

void Interface::joint1CW() {
	Interface::getInstance()->robot->joint1Adjust(-1 * Interface::getInstance()->robot->joint_rotate_amount);
}

void Interface::joint2CCW() {
	Interface::getInstance()->robot->joint2Adjust(Interface::getInstance()->robot->joint_rotate_amount);
}

void Interface::joint2CW() {
	Interface::getInstance()->robot->joint2Adjust(-1 * Interface::getInstance()->robot->joint_rotate_amount);
}



void Interface::paintBrushSlideLeft() {
	Interface::getInstance()->robot->paintBrushXAdjust(-1 * Interface::getInstance()->robot->paint_slide_amount);
}

void Interface::paintBrushSlideRight() {
	Interface::getInstance()->robot->paintBrushXAdjust(Interface::getInstance()->robot->paint_slide_amount);
}

void Interface::paintBrushSlideUp() {
	Interface::getInstance()->robot->paintBrushYAdjust(Interface::getInstance()->robot->paint_slide_amount);
}

void Interface::paintBrushSlideDown() {
	Interface::getInstance()->robot->paintBrushYAdjust(-1 * Interface::getInstance()->robot->paint_slide_amount);
}


void Interface::increaseStep() {
	Interface::getInstance()->robot->joint_rotate_amount++;
	Interface::getInstance()->robot->joint_slide_amount++;
	Interface::getInstance()->robot->paint_slide_amount++;
	Interface::getInstance()->graphics->paint_radius++;
}

void Interface::decreaseStep() {
	Interface::getInstance()->robot->joint_rotate_amount--;
	Interface::getInstance()->robot->joint_slide_amount--;
	Interface::getInstance()->robot->paint_slide_amount--;
	Interface::getInstance()->graphics->paint_radius--;
}


//serverGUIthread function
void Interface::serverFinish() {
	// This is called after the server is finished interpreting the commands from the client
	// This just needs to update the interface graphically. All robot motions should be final

	while (true) {
		DWORD mResult;

		char* new_command = "VOID";

		mResult
			= WaitForSingleObject(
				Interface::getInstance()->robot->ghMutex,    // handle to mutex
				INFINITE);  // no time-out interval

		switch (mResult)
		{
			// The thread got ownership of the mutex
		case WAIT_OBJECT_0:
			try {
				//pull from the command queue
				if (!Interface::getInstance()->robot->commands.empty()) {
					new_command = Interface::getInstance()->robot->commands.front();

					//remove command
					Interface::getInstance()->robot->commands.pop();
				}

				// Release ownership of the mutex object
				if (!ReleaseMutex(Interface::getInstance()->robot->ghMutex))
				{
					// Handle error.
				}
			}

			catch(exception e) {
				//expections...
			}
			break;

		}

		//new command found
		if (new_command != "VOID") {
			
			//Move up
			if (new_command[0] == 'U') {
				printf("Moving up...\n");
				Interface::getInstance()->robot->paintBrushYAdjust(5);
			}

			//Move right
			if (new_command[0] == 'R') {
				printf("Moving right...\n");
				Interface::getInstance()->robot->paintBrushXAdjust(5);
			}


			//Move down
			if (new_command[0] == 'D') {
				printf("Moving down...\n");
				Interface::getInstance()->robot->paintBrushYAdjust(-5);
			}


			//Move Left
			if (new_command[0] == 'L') {
				printf("Moving left...\n");
				Interface::getInstance()->robot->paintBrushXAdjust(-5);
			}


			//Increase (+)
			if (new_command[0] == '+') {
				printf("increase...\n");
				Interface::increaseStep();
			}


			//Decrease(-)
			if (new_command[0] == '-') {
				printf("decrease...\n");
				Interface::decreaseStep();
			}


			//Paint
			if (new_command[0] == 'P') {
				printf("Painting...\n");
				Interface::paintAction();
			}


			//Forward Kinematics
			if (new_command[0] == 'C') {
				
				if (new_command[1] == 'W') {

					//CW1
					if (new_command[2] == '1') {

						Interface::joint1CW();

					}

					//CW2
					if (new_command[2] == '2') {

						Interface::joint2CW();

					}

				}

				if (new_command[1] == 'C') {

					//CCW1
					if (new_command[3] == '1') {

						Interface::joint1CCW();

					}

					//CCW2
					if (new_command[3] == '2') {

						Interface::joint2CCW();

					}

				}

			}


			//Slide commands
			if (new_command[0] == 'S') {

				//Slide right
				if (new_command[6] == 'r') {

					Interface::getInstance()->robot->paintBrushXAdjust(5);

				}

				//Slide left
				if (new_command[6] == 'l') {

					Interface::getInstance()->robot->paintBrushXAdjust(-5);

				}

			}
		}

		//delay if active
		if (Interface::getInstance()->simulate_delay)
			Sleep(2000);

		//update graphics
		Interface::getInstance()->drawUpdate();
		glutPostRedisplay();
	}

}
