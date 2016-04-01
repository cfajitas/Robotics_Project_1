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

	ghMutex = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);             // unnamed mutex

	delay_active = false;

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
	//radians = deg2rad(theta2);
	//paintx = joint2x + (length2*cos(radians));
	//painty = joint2y + (length2*sin(radians));
	joint2Adjust(amount);
}

void Robot::joint2Adjust(float amount) {
	theta2 += amount;
	float radians = deg2rad(theta2);
	paintx = joint2x + (length2*cos(radians));
	painty = joint2y + (length2*sin(radians));
}

void Robot::paintBrushXAdjust(float amount) {
	
	joint0Adjust(amount);
	printf("ROBOT CALLED!!!! value = %d", joint0x);

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

/*****************************************************
	       INVERSE KINEMATICS FOR ROBOT
******************************************************/
float dist(float x1, float y1, float x2, float y2) {
	return sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
}

void Robot::inverse_kinematics(float px, float py, float alpha) {

	// get all info about robot before computing
	get_robot_info();
	cout << "Given (px, py, alpha): ";
	cout << "(" << px << ", " << py << ", " << alpha << ")" << endl;
	cout << "Find d1, theta1, and theta2..." << endl << endl;

	// initialize variables
	float a1 = 150, a2 = 100, a3 = 75;	// arm lenghts
	float theta1_initial = theta1;
	float theta2_initial = theta2;

	// start kinematics computations
	float radians = deg2rad(theta1);
	float radians2 = deg2rad(alpha);
	float x4 = joint1x + (a2*cos(radians)) + (a3*cos(radians2));
	float y4 = joint1y + (a2*sin(radians)) + (a3*sin(radians2));
	cout << "<< x4 << " << x4 << endl;
	cout << "<< y4 << " << y4 << endl;

	radians = (y4 - a1 - (a3*sin(radians2))) / a2;
	theta1 = acos(radians);
	alpha = theta1 + theta2;
	theta2 = alpha - theta1;
	cout << "<< new theta1 << " << theta1 << endl;
	cout << "<< new theta2 << " << theta2 << endl;
	cout << "<< new alpha << " << alpha << endl;

	radians = deg2rad(theta1);
	radians2 = deg2rad(alpha);
	float d1 = x4 - (a2*cos(radians) - (a3*cos(radians2)));
	cout << "<< new d1 << " << d1 << endl;

}

void Robot::get_robot_info() {
	cout << "Coordinates of robot positions: " << endl;
	cout << "-------------------------------" << endl;
	cout << "Joint0x :" << joint0x << endl;
	cout << "Joint0y :" << joint0y << endl;
	cout << "Joint1x :" << joint1x << endl;
	cout << "Joint1y :" << joint1y << endl;
	cout << "Joint2x :" << joint2x << endl;
	cout << "Joint2y :" << joint2y << endl;
	cout << "Paintx :" << paintx << endl;
	cout << "Painty :" << painty << endl << endl;

	cout << "Angles for robot positions: " << endl;
	cout << "-------------------------------" << endl;
	cout << "Theta 1: " << theta1 << endl;
	cout << "Theta 2: " << theta2 << endl << endl;
};


void Robot::paintBrushYAdjust(float amount) {
	if (reachable(paintx, painty + amount)) {
		painty += amount;

		//inverse_kinematics(paintx, painty, theta1 + theta2);
		
		
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
