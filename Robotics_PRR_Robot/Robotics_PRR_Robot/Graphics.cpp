#include "Graphics.h"

Graphics::Graphics(int argc, char ** argv, int width, int height, int start_x, int start_y, const char * window_name) {

	paint_radius = 10.0f;

	screen_width = width;
	screen_height = height;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(start_x, start_y);

	glutCreateWindow(window_name);
}






void Graphics::writeText(void* font, char* text, float x, float y) {
	glRasterPos2f(x, y);
	while (*text != '\0') {
		glutBitmapCharacter(font, *text);
		++text;
	}
}

void Graphics::drawButton(Button * b) {

	// Draws a button 'b' and writes the necessary text of the button

	glColor3f(0.6f, 0.6f, 0.6f);
	glBegin(GL_POLYGON);
	glVertex2f(b->x, b->y);
	glVertex2f(b->x, b->y + b->h);
	glVertex2f(b->x + b->w, b->y + b->h);
	glVertex2f(b->x + b->w, b->y);
	glEnd();

	float text_length = ( (float) glutBitmapLength(GLUT_BITMAP_HELVETICA_10, reinterpret_cast<const unsigned char*>(b->label)) / screen_width)*2;
	float x_offset = (button_width - text_length) / 2.0f;
	float xx = b->x + x_offset;
	float yy = b->y + (button_height / 2.0f) - 0.01f;

	glColor3f(0.1f, 0.1f, 0.1f);
	writeText(GLUT_BITMAP_HELVETICA_10, b->label, xx, yy);
}

void Graphics::drawButtons(vector<Button*> buttons) {
	for (Button* button : buttons) {
		drawButton(button);
	}
}

void Graphics::positionButtons(vector<Button*> buttons) {

	// This determines the x and y coordinates of each of the buttons so that they can be accurately drawn to the screen

	float horizontal_spacing = button_width + inner_spacing;
	float vertical_spacing = button_height + inner_spacing;


	float x0	= -1 + outer_spacing + inner_spacing;
	float y0	= -1 + outer_spacing + inner_spacing;
	
	float x1 = 1 - outer_spacing - inner_spacing - button_width;
	float x5 = x1 - (horizontal_spacing / 2);
	float y5 = y0 + (vertical_spacing / 2);


	Button* jointLeft = buttons[0];
		// Bottom left corner
		jointLeft->x = x0;
		jointLeft->y = y0;
		jointLeft->w = button_width;
		jointLeft->h = button_height;

	Button* jointRight = buttons[1];
		jointRight->x = x0 + horizontal_spacing;
		jointRight->y = y0;
		jointRight->w = button_width;
		jointRight->h = button_height;

	Button* joint1CCW = buttons[2];
		joint1CCW->x = x0;
		joint1CCW->y = y0 + vertical_spacing;
		joint1CCW->w = button_width;
		joint1CCW->h = button_height;

	Button* joint1CW = buttons[3];
		joint1CW->x = x0 + horizontal_spacing;
		joint1CW->y = y0 + vertical_spacing;
		joint1CW->w = button_width;
		joint1CW->h = button_height;

	Button* joint2CCW = buttons[4];
		joint2CCW->x = x0;
		joint2CCW->y = y0 + vertical_spacing + vertical_spacing;
		joint2CCW->w = button_width;
		joint2CCW->h = button_height;

	Button* joint2CW = buttons[5];
		joint2CW->x = x0 + horizontal_spacing;
		joint2CW->y = y0 + vertical_spacing + vertical_spacing;
		joint2CW->w = button_width;
		joint2CW->h = button_height;

	Button* painter = buttons[6];
	painter->x = x0 + horizontal_spacing * 3;
	painter->y = y0 + vertical_spacing * 2;
	painter->w = button_width;
	painter->h = button_height;

	Button* leftSlide = buttons[7];
	leftSlide->x = x1 - button_width - button_width;
	leftSlide->y = y0 + button_height;
	leftSlide->w = button_width;
	leftSlide->h = button_height;

	Button* upSlide = buttons[8];
	upSlide->x = x1 - button_width;
	upSlide->y = y0 + button_height + button_height;
	upSlide->w = button_width;
	upSlide->h = button_height;

	Button* downSlide = buttons[9];
	downSlide->x = x1 - button_width;
	downSlide->y = y0;
	downSlide->w = button_width;
	downSlide->h = button_height;

	Button* rightSlide = buttons[10];
	rightSlide->x = x1;
	rightSlide->y = y0 + button_height;
	rightSlide->w = button_width;
	rightSlide->h = button_height;

	Button* increment = buttons[11];
	increment->x = painter->x;
	increment->y = painter->y - vertical_spacing;
	increment->w = button_width;
	increment->h = button_height;

	Button* decrement = buttons[12];
	decrement->x = painter->x;
	decrement->y = painter->y - vertical_spacing - vertical_spacing;
	decrement->w = button_width;
	decrement->h = button_height;



}

void Graphics::drawBackground() {
	glClearColor(0, 0, 0, 0);	

	// Primary draw-space (where the robot will be drawn)
	glBegin(GL_POLYGON);
	glColor3f(0.95f, 0.95f, 0.95f);
	glVertex2f(-1 + outer_spacing,		-1 + outer_spacing + outer_spacing + secondary_width);
	glVertex2f(-1 + outer_spacing,		 1 - outer_spacing);
	glVertex2f(1 - outer_spacing,		 1 - outer_spacing);
	glVertex2f(1 - outer_spacing,		-1 + outer_spacing + outer_spacing + secondary_width);
	glEnd();

	// Secondary draw-space (where the buttons will be drawn)
	glBegin(GL_POLYGON);
	glColor3f(0.95f, 0.95f, 0.95f);
	glVertex2f(-1 + outer_spacing,		-1 + outer_spacing);
	glVertex2f(-1 + outer_spacing,		-1 + outer_spacing + secondary_width);
	glVertex2f(1 - outer_spacing,		-1 + outer_spacing + secondary_width);
	glVertex2f(1 - outer_spacing,		-1 + outer_spacing);
	glEnd();
}

void Graphics::recalculatePoints(float* data) {
	// Max reach = 325
	float x_offset = 0;
	float y_offset = 1 - primary_width + outer_spacing;

	for (int i = 0; i < 8; i++) {
		data[i] = data[i] / screen_width;
		if (i % 2 == 0) {
			// x_coordinate
			data[i] = data[i] + x_offset;
		}
		else {
			// y_coordinate
			data[i] = data[i] + y_offset;
		}
	}
}

void Graphics::drawRobot(float* data) {
	recalculatePoints(data);

	float line_width = 2.5f;
	float color[3] = { 0.5f, 0.5f, 0.5f };

	glLineWidth(line_width);
	glColor3f(color[0], color[1], color[2]);
	glBegin(GL_LINES);
	glVertex2f(data[0], data[1]);
	glVertex2f(data[2], data[3]);
	glEnd();

	glLineWidth(line_width);
	glColor3f(color[0], color[1], color[2]);
	glBegin(GL_LINES);
	glVertex2f(data[2], data[3]);
	glVertex2f(data[4], data[5]);
	glEnd();

	glLineWidth(line_width);
	glColor3f(color[0], color[1], color[2]);
	glBegin(GL_LINES);
	glVertex2f(data[4], data[5]);
	glVertex2f(data[6], data[7]);
	glEnd();
}

void Graphics::addPaint(float x, float y) {
	x = x / screen_width;
	y = y / screen_width;
	paint_locations.push_back(x);
	paint_locations.push_back(y);
}

struct Point {
	float x;
	float y;
	Point(float xx, float yy) {
		x = xx;
		y = yy;
	}
	void normalize() {
		float magnitude = sqrt( (x*x) + (y*y) );
		if (magnitude == 0) return;
		x = x / magnitude;
		y = y / magnitude;
	}
	Point pixelate(float radius, float reduction) {
		x *= radius / reduction;
		y *= radius / reduction;
		return *this;
	}
	Point offset(float xx, float yy) {
		x += xx;
		y += yy;
		return *this;
	}
};

Point rotate(float x, float y, float angle) {
	float xx = x;
	float yy = y;

	x = (xx * cos(angle)) - (yy * sin(angle));
	y = (xx * sin(angle)) + (yy * cos(angle));
	Point p = Point(x, y);
	p.normalize();
	return p;
}

void Graphics::drawPaint() {
	for (int i = 0; i < paint_locations.size(); i += 2) {
		float x_offset = paint_locations[i];
		float y_offset = paint_locations[i + 1] - outer_spacing;

		float x = 0;
		float y = 1;

		for (float angle = 0; angle < 180; angle++) {
			Point x0 = rotate(x, y, angle).pixelate(paint_radius, screen_width).offset(x_offset, y_offset);
			Point x1 = rotate(x, y, angle + 90).pixelate(paint_radius, screen_width).offset(x_offset, y_offset);
			Point x2 = rotate(x, y, angle + 180).pixelate(paint_radius, screen_width).offset(x_offset, y_offset);

			glColor3f(0.5f, 0.5f, 0.5f);
			glBegin(GL_TRIANGLES);
			glVertex2f(x0.x, x0.y);
			glVertex2f(x1.x, x1.y);
			glVertex2f(x2.x, x2.y);
			glEnd();
		}
	}
}