#include "stdafx.h"
#include "Button.h"


Button::Button(char* label, ButtonCallback onClick) {

	this->x = this->y = 0;
	this->w = this->h = 1;

	this->label = label;
	this->onClick = onClick;
}

char* Button::getLabel() {
	return label;
}
