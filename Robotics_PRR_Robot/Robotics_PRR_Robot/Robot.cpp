#include "Robot.h"


Robot::Robot() {
	joint_slide_amount = joint_rotate_amount = paint_slide_amount = 5;

	joint0x = 0;
	joint0y = 0;

	joint1x = 0;
	joint1y = 150;

	joint2x = 0;
	joint2y = 250;

	paintx  = 0;
	painty  = 325;

	theta2 = 90;
	theta1 = 90;
}

// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~
// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~
// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~

// Testing values to make sure the robot can be drawn with updated values
void Robot::joint0Adjust(float amount) {
	joint0x += amount;
	joint1x += amount;
	joint2x += amount;
	paintx += amount;
}
void Robot::joint1Adjust(float amount) {
	theta1 += amount;
	float radians = deg2rad(theta1);
	joint2x = joint1x + (length1*cos(radians));
	joint2y = joint1y + (length1*sin(radians));
	radians = deg2rad(theta2);
	paintx = joint2x + (length2*cos(radians));
	painty = joint2y + (length2*sin(radians));
}
void Robot::joint2Adjust(float amount) {
	theta2 += amount;
	float radians = deg2rad(theta2);
	paintx = joint2x + (length2*cos(radians));
	painty = joint2y + (length2*sin(radians));
}

void Robot::paintBrushXAdjust(float amount) {
	
	joint0Adjust(amount);

	//Using sliding for left and right inverse kinematics renders
	//all code below irrevlant

	/*
	if (reachable(paintx + amount, painty)) {
		paintx += amount;

		//float phi = rad2deg(acos(deg2rad((paintx*paintx+painty*painty+length1*length1-length2*length2)/(2*length1*sqrtf(paintx*paintx+painty*painty)))));
		float paintxT = paintx - joint1x;
		float paintyT = painty - joint1y;
		float phi = rad2deg(acos(deg2rad((paintxT*paintxT + painty*painty + length1*length1 - length2*length2) / (2 * length1*sqrtf(paintxT*paintxT + paintyT*paintyT)))));

		float beta = rad2deg(atan2(painty, paintx));
		float angle1 = beta - phi;
		//float omega = rad2deg(acos(deg2rad((paintx*paintx+painty*painty-length1*length1-length2*length2)/(2*length1*length2))));
		//float alpha = 180 - omega;

		if (amount > 0)
			theta1 += -angle1;
		else
			theta1 += angle1;

		float radians = deg2rad(theta1);
		joint2x = joint1x + (length1*cos(radians));
		joint2y = joint1y + (length1*sin(radians));
		
		//radians = deg2rad(theta2+=alpha);
		//paintx = joint2x + (length2*cos(radians));
		//painty = joint2y + (length2*sin(radians));
		
	}
	*/
}

void Robot::paintBrushYAdjust(float amount) {
	if (reachable(paintx, painty + amount)) {
		painty += amount;

		//float phi = rad2deg(acos(deg2rad((paintx*paintx+painty*painty+length1*length1-length2*length2)/(2*length1*sqrtf(paintx*paintx+painty*painty)))));
		float paintxT = paintx - joint1x;
		float paintyT = painty - joint1y;
		float phi = rad2deg(acos(deg2rad((paintxT*paintxT + painty*painty + length1*length1 - length2*length2) / (2 * length1*sqrtf(paintxT*paintxT + paintyT*paintyT)))));

		float beta = rad2deg(atan2(painty,paintx));
		float angle1 = beta - phi;
		//float omega = rad2deg(acos(deg2rad((paintx*paintx+painty*painty-length1*length1-length2*length2)/(2*length1*length2))));
		//float alpha = 180 - omega;

		if (amount < 0)
			theta1 += -angle1;
		else
			theta1 += angle1;

		float radians = deg2rad(theta1);
		joint2x = joint1x + (length1*cos(radians));
		joint2y = joint1y + (length1*sin(radians));
		/*
		radians = deg2rad(theta2+=alpha);
		paintx = joint2x + (length2*cos(radians));
		painty = joint2y + (length2*sin(radians));
		*/
	}
}

//checks if point is within reachable workspace
bool Robot::reachable(float x, float y) {
	float distance = sqrt((x-joint1x)*(x-joint1x) + (y-joint1y)*(y-joint1y));
	if (distance > length1 + length2)
		return false;
	if (distance < length1 - length2)
		return false;
	return true;
}

float Robot::rad2deg(float n) {
	return (n * 180) / M_PI;
}

float Robot::deg2rad(float n) {
	return (n*M_PI) / 180;
}
