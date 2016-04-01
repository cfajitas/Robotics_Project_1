#ifndef ROBOT_H
#define ROBOT_H

#define _USE_MATH_DEFINES
#include<stdlib.h>
#include<math.h>
#include<vector>
#include<queue>

#include <Windows.h>

//cout for testing purposes
#include<iostream>

using namespace std;

class Robot {
public:

	// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~
	// =~=~=~  VARIABLES  ~=~=~=~=~=~=~=~
	// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~

	// The Interface class reads these values and sends them to the adjusting functions
	// These three values update on user input
	float joint_slide_amount;
	float joint_rotate_amount;
	float paint_slide_amount;

	// Length of Robot Arms
	const float length1 = 100;
	const float length2 = 75;

	// These values are in pixels
	float joint0x;
	float joint0y;

	float joint1x;
	float joint1y;

	float joint2x;
	float joint2y;

	float paintx;
	float painty;

	// Angles between rotating joints
	float theta2;
	float theta1;

	//queue of commands from the server
	queue<char*> commands;

	// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~
	// =~=~=~  CONSTRUCTORS  =~=~=~=~=~=~
	// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~

	Robot();

	// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~
	// =~=~=~  FUNCTIONS  ~=~=~=~=~=~=~=~
	// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~

	// Rather than have separate counter-clock-wise and clock-wise functions, they are combined into a single value
	// 'amount' is either +slide_amount or -slide_amount (the same for rotate_amount)
	void joint0Adjust(float amount);
	void joint1Adjust(float amount);
	void joint2Adjust(float amount);

	void paintBrushXAdjust(float amount);
	void paintBrushYAdjust(float amount);
	
	// Helper functions
	bool reachable(float x, float y);
	float rad2deg(float n);
	float deg2rad(float n);

	//MUTEX to allow synchronzation of server and client threads
	HANDLE ghMutex;

};

#endif