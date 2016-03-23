#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdlib.h>
#include <GL/glut.h>

#include "Button.h"
#include <vector>

using namespace std;

class Graphics
{
public:

	// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~
	// =~=~=~  VARIABLES  ~=~=~=~=~=~=~=~
	// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~

	const float primary_width = (0.6f) * 2;
	const float secondary_width = (0.3f) * 2;
	const float outer_spacing = (2.0f - primary_width - secondary_width) / 2.0f;
	const float inner_spacing = 0.05f;
	const float button_width = (2 - (2 * outer_spacing) - (5 * inner_spacing))/8.0f;
	const float button_height = (secondary_width - (4 * inner_spacing))/3.0f;

	// ~ Updates on user input ~
	float paint_radius;

	int screen_width;
	int screen_height;

	vector<float> paint_locations;

	// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~
	// =~=~=~  CONSTRUCTORS  =~=~=~=~=~=~
	// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~

	Graphics(int argc, char ** argv, int width, int height, int start_x, int start_y, const char * window_name);

	// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~
	// =~=~=~  FUNCTIONS  ~=~=~=~=~=~=~=~
	// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~

	void drawBackground();

	void writeText(void* font, char* text, float x, float y);
	void drawButton(Button* button);
	void drawButtons(vector<Button*> buttons);
	void positionButtons(vector<Button*> buttons);

	void addPaint(float x, float y);
	void drawPaint();

	void recalculatePoints(float* data);
	void drawRobot(float* data);
};

#endif