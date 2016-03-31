#pragma once

//typedef void(*ButtonCallback)();

typedef void (*ButtonCallback)();

class Button {
public:
	float x;
	float y;
	float w;
	float h;
	char* label;
	ButtonCallback onClick;

	Button(char* label, ButtonCallback onClick);

	char* getLabel();
};