/*
 * NavigationController.cpp
 *
 *  Created on: Mar 12, 2016
 *      Author: Peter
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <cmath>
#include "../../include/enums.h"
#include "../../include/globals.h"
#include "../include/system/console.h"


#include "../../include/controllers/MotorController.h"
#include "../../include/controllers/NavigationController.h"

using namespace std;
int angle_in_radians = 0; // Change this value from 0 to output the angle in radians

namespace blaze {

MotorController *NavigationController::motorController = 0 ;

/*
 * Constructor
 */
NavigationController::NavigationController(MotorController *mc){
	motorController = mc;
	this->init();
}

void NavigationController::addMotorController(MotorController *mc){
	motorController = mc;
}

void NavigationController::init(){

	lidar = lidarIO(LIDAR_PORT);
	lidar.connect();

	/**
	 * Added by Terance
	 */
	this->currentFrontPos.MaximumAngle = FRONT_MAX;
	this->currentFrontPos.MinimumAngle = FRONT_MIN;
	this->currentBackPos.MaximumAngle = BACK_MAX;
	this->currentBackPos.MinimumAngle = BACK_MIN;
	this->currentRightPos.MaximumAngle = RIGHT_MAX;
	this->currentRightPos.MinimumAngle = RIGHT_MIN;
	this->currentLeftPos.MaximumAngle = LEFT_MAX;
	this->currentLeftPos.MinimumAngle = LEFT_MIN;

}

NavigationController::~NavigationController() {
	// TODO Auto-generated destructor stub
}

/**
 * Function for stopping navigation controller
 */
void NavigationController::stopNow(){

	//TODO: implement method
	lidar.disconnect();
}


/**
 * Funtion used to determine which port is being played
 */
PortConfig NavigationController::determinePort(){
	console::debug("NavigationController::determinePort() start");

	try {
		double data[370] ;
		int left_max; int right_max; int i;
		left_max = 0;
		right_max = 0;

		//lidarIO lidar(LIDAR_PORT);
		//lidar.connect();
		console::debug("LIDAR: get Data");
		lidar.getData(lidar.lidarFileDescriptor, data);

		for (i=17; i<=50; i++){
			if(data[i] > left_max) {
				left_max = data[i];
			}
		}

		for(i=285; i<=343; i++){
			if(data[i] > right_max){
				right_max = data[i];
			}
		}

		cout << "left max " + to_string(left_max) << endl;
		cout << "right max: " + to_string(right_max) << endl;


		if(left_max > right_max){
			return PortConfig::B;
		}
		if( right_max > left_max ){
			return PortConfig::A;
		}
	}catch (int err){
		console::error("Error occured in NavigationController: " + to_string(err));
	}

	return PortConfig::UNDEFINED;
}


void NavigationController::updateLocation(){
	console::debug("NAV: Update Location");
	lidar.getData(lidar.lidarFileDescriptor, data);
	this->offset_correction();

	this->calculateAverage(&currentFrontPos);
	this->calculateMinimum(&currentFrontPos);
	this->calculateMaximum(&currentFrontPos);
	this->calculateTheta(&currentFrontPos);

	this->calculateAverage(&currentBackPos);
	this->calculateMinimum(&currentBackPos);
	this->calculateMaximum(&currentBackPos);
	this->calculateTheta(&currentBackPos);

	this->calculateAverage(&currentRightPos);
	this->calculateMinimum(&currentRightPos);
	this->calculateMaximum(&currentRightPos);
	this->calculateTheta(&currentRightPos);

	this->calculateAverage(&currentLeftPos);
	this->calculateMinimum(&currentLeftPos);
	this->calculateMaximum(&currentLeftPos);
	this->calculateTheta(&currentLeftPos);
}


/* Values have been adjusted to set due north of the bot to 90 degrees */
/* Back values added with 90 degrees accounted for                     */
double NavigationController::getPosition (Face f, DistType dt){

	console::debug("LIDAR: get Data");
	lidar.getData(lidar.lidarFileDescriptor, data);


	this->calculateAverage(&currentFrontPos);
	this->calculateMinimum(&currentFrontPos);
	this->calculateMaximum(&currentFrontPos);
	this->calculateTheta(&currentFrontPos);

	this->calculateAverage(&currentBackPos);
	this->calculateMinimum(&currentBackPos);
	this->calculateMaximum(&currentBackPos);
	this->calculateTheta(&currentBackPos);

	this->calculateAverage(&currentRightPos);
	this->calculateMinimum(&currentRightPos);
	this->calculateMaximum(&currentRightPos);
	this->calculateTheta(&currentRightPos);

	this->calculateAverage(&currentLeftPos);
	this->calculateMinimum(&currentLeftPos);
	this->calculateMaximum(&currentLeftPos);
	this->calculateTheta(&currentLeftPos);


	int index = 0;
	int size = 0;
	double sum = 0;
	double count = 0.0;
	double resultantValue = 0.0;
	double maximum = 0.0;
	double minimum = 5000.0;
	int maxIndex = 0;
	int minIndex = 0;
	int i; int j; int k;

	switch(f){
		case Face::Front :
			i = 140;	j = 42;		break;

		case Face::Back :

			i = 302;	j = 238;	break;
		case Face::Right :

			i = 29;		j = -27;	break;

		case Face::Left :

			i = 205;	j = 155;	break;

		case Face::Front_L :	i = 140;	j = 107;	break;
		case Face::Front_C : 	i = 106; 	j = 74;		break;
		case Face::Front_R: 	i = 75; 	j = 42;		break;
		case Face::Back_L :		i = 309;	j = 284;	break;
		case Face::Back_C :		i = 283;	j = 258;	break;
		case Face::Back_R :		i = 257;	j = 231;	break;
    }

	for (k=i; k>=j; k--){
		if (k<=0){
			index = 360+k;
		} else{
			index = k;
		}

		switch (dt) {
			case DistType::Avg :
				sum += data[index];
				if(data[index] != 0){
					count++;
				}
				break;
			case DistType::Min :
				if ((data[index]<minimum) && (data[index] != 0)){
					 minimum = data[index];
					minIndex = index;
				}
				break;
			case DistType::Max :
				if (data[index]>maximum){
					maximum = data[index];
					maxIndex = index;
				}
				break;
			case DistType::Theta :

				if ((data[index]<minimum) && (data[index] != 0)){
					minimum = data[index];
					minIndex = index;
				}
				break;
		} //end of switch
    } //end of for

	switch (dt) {
		case DistType::Avg :
			resultantValue = sum/count;
			break;
		case DistType::Min :
			resultantValue = minimum;
			break;
		case DistType::Max :
			resultantValue = maximum;
			break;
		case DistType::Theta :

			switch (f)	{
			case Face::Left:
				resultantValue = minIndex - 180;
				break;
			case Face::Right :
				//cout << "min: " + to_string(minimum);
				if(minIndex<=0){
					resultantValue = minIndex;
				}else{
					resultantValue = minIndex - 360;
				}
				break;
			case Face::Front :
				//cout << "index: " + to_string(index);
				cout << endl;
				resultantValue = minIndex - 90;
				break;
			case Face::Back :
				resultantValue = minIndex - 270;
				break;
			}
	}

	if(angle_in_radians != 0){
		resultantValue *= 0.0174533;
	}




	console::debug("NC: getPositional Data: " + to_string(resultantValue));
	return resultantValue;
}

/*
 * Used to calculate the Minimum distance in position object
 */
void NavigationController::calculateMinimum(angleRange *p){
	int index;
	double minimum = 5000.0;
	int minIndex = 0;

	for (int k=p->MaximumAngle; k>=p->MinimumAngle; k--){
			if (k<=0){
				index = 360+k;
			} else{
				index = k;
			}
			if ((data[index]<minimum) && (data[index] != 0)){
				minimum = data[index];
				minIndex = index;
			}

	}
	p->minimumDistance = minimum;
	return ;
}
/*
 * Used to calculate the Maximum distance in position object
 */
void NavigationController::calculateMaximum(angleRange *p){
	int index;
	double maximum = 0;
	int maxIndex = 0;

	for (int k=p->MaximumAngle; k>=p->MinimumAngle; k--){
			if (k<=0){
				index = 360+k;
			} else{
				index = k;
			}
				if (data[index]>maximum){
					maximum = data[index];
					maxIndex = index;
				}
	}
	p->maximumDistance = maximum;
	return ;
}

/*
 * Used to calculate the Average distance in position object
 */
void NavigationController::calculateAverage(angleRange *p){
	int index;
	double sum;
	double count;


	for (int k=p->MaximumAngle; k>=p->MinimumAngle; k--){
			if (k<=0){
				index = 360+k;
			} else{
				index = k;
			}
			sum += data[index];
			if(data[index] != 0){
				count++;
			}

	}

	p->averageDistance = sum/count;
	return ;
}

/*
 * Used to calculate the Theta distance in position object
 */
void NavigationController::calculateTheta(angleRange *p){
	int index;
	double maximum = 0;
	int maxIndex = 0;
	int minIndex = 0;
	double resultantValue;
	double minimum = 5000.0;


	for (int k=p->MaximumAngle; k>=p->MinimumAngle; k--){
			if (k<=0){
				index = 360+k;
			} else{
				index = k;
			}
			if ((data[index]<minimum) && (data[index] != 0)){
				minimum = data[index];
				minIndex = index;
			}
	}

	if(	p->MaximumAngle==LEFT_MAX){
		resultantValue = minIndex - 180;
	}else if(p->MaximumAngle == RIGHT_MAX){
		if(minIndex<=0){
			resultantValue = minIndex;
		}else{
			resultantValue = minIndex - 360;
		}
	}else if(p->MaximumAngle==FRONT_MAX){
		resultantValue = minIndex - 90;
	}else{
		resultantValue = minIndex - 270;
	}
	if(angle_in_radians != 0){
		resultantValue *= 0.0174533;
	}
	p->angleOffset = resultantValue;
	return ;
}


/*
 * Function for aligning the robot to the
 */
void NavigationController::alignToFace(Face f){
	console::debug("NC: alignToFace");
	double t = this->getPosition(f, DistType::Theta);


	//while the absolute value of theta is greater than one...
	while(abs(t) >5){
		if(t>1){
			console::debug("NC: Angle is positive, needs counter-clockwise alignment : ");
			//counter-clock
			motorController->engage(1,SPEED::QUARTER, MOVEMENT::FORWARD);
			motorController->engage(2,SPEED::QUARTER, MOVEMENT::REVERSE);
		}
		if(t < -1){
			//clockwise
			console::debug("NC: Angle is negative, needs clockwise alignment : ");
			motorController->engage(2,SPEED::QUARTER, MOVEMENT::FORWARD);
			motorController->engage(1,SPEED::QUARTER, MOVEMENT::REVERSE);
		}

		t = this->getPosition(f, DistType::Theta);

		motorController->stopNow();

	}

	motorController->stopNow();



}


/**
 * Method for correcting offset values in the lidar position
 */
void NavigationController::offset_correction(){
	double x1;
	double y1;
	double dt;
	int theta;
	//int theta_prime;
	//double updated_data[360];
	int i;
	double theta_q1 = i*0.0174533; //radians
	double theta_q2 = (180 - i)*0.0174533;
	double theta_q3 = (i -180)*0.0174533;
	double theta_q4 = (360 -i)*0.0174533;

	for(i=0; i<90; i++){

		x1=(data[i]*cos(theta)) + (-39);
		y1=(data[i]*sin(theta)) + (41);
		dt = sqrt(pow(x1,2)+pow(y1,2));
		data[i] = dt;
		//theta_prime = (int) (acos(x1/dt)/0.0174533);
		//updated_data[theta_prime] = dt;
	}

	for(i=90; i<180; i++){
		theta = (180 - i)*0.0174533;
		x1=(data[i]*cos(theta)) + (-39);
		y1=(data[i]*sin(theta)) + (41);
		dt = sqrt(pow(x1,2)+pow(y1,2));
		data[i] = dt;
		//theta_prime = (int) (180 - (acos(x1/dt)/0.0174533));
		//updated_data[theta_prime] = dt;
	}
	for(i=180; i<270; i++){
		theta = (i -180)*0.0174533;
		x1=(data[i]*cos(theta)) + (-39);
		y1=(data[i]*sin(theta)) + (41);
		dt = sqrt(pow(x1,2)+pow(y1,2));
		data[i] = dt;
		//theta_prime = (int)((acos(x1/dt)/0.0174533) + 180);
		//updated_data[theta_prime] = dt;
	}
	for(i=270; i<360; i++){
		theta = (360 -i)*0.0174533;
		x1=(data[i]*cos(theta)) + (-39);
		y1=(data[i]*sin(theta)) + (41);
		dt = sqrt(pow(x1,2)+pow(y1,2));
		data[i] = dt;
		//theta_prime = (int)(360 - (acos(x1/dt)/0.0174533));
		//updated_data[theta_prime] = dt;
	}
}




/**
 * Function for navigating through the tunnel
 */
void NavigationController::navigateThroughTunnel(){

	//move forward follow wall until 60 - 65 cm from the rear wall...



}


void NavigationController::moveUntil(int forward_target_distance, MOVEMENT dir  ){

	this->updateLocation();


	angleRange ar;
	ar.MaximumAngle = 100;
	ar.MinimumAngle = 80;

	this->calculateAverage(&ar);


	console::debug ("Front Location Average " + to_string(ar.averageDistance));

	if(dir == MOVEMENT::FORWARD){
		while(ar.averageDistance > forward_target_distance){

			this->updateLocation();

			if(ar.averageDistance < forward_target_distance){
				motorController->stopNow();
			}else{
				motorController->engage(1,SPEED::HALF, dir);
				motorController->engage(2,SPEED::HALF, dir);
			}
		}

	}else{
		while(ar.averageDistance < forward_target_distance){
			this->updateLocation();


			if(ar.averageDistance > forward_target_distance){
				motorController->stopNow();
			}else{
				motorController->engage(1,SPEED::HALF, dir);
				motorController->engage(2,SPEED::HALF, dir);
			}
		}
	}


/*
	console::debug ("Front Location Average " + to_string(this->currentFrontPos.averageDistance));

	if(dir == MOVEMENT::FORWARD){
		while(this->currentFrontPos.averageDistance > forward_target_distance){

			this->updateLocation();

			if(this->currentFrontPos.averageDistance < forward_target_distance){
				motorController->stopNow();
			}else{
				motorController->engage(1,SPEED::FIVE_EIGHTH, dir);
				motorController->engage(2,SPEED::FIVE_EIGHTH, dir);
			}
		}

	}else{
		while(this->currentFrontPos.averageDistance < forward_target_distance){
			this->updateLocation();


			if(this->currentFrontPos.averageDistance > forward_target_distance){
				motorController->stopNow();
			}else{
				motorController->engage(1,SPEED::FIVE_EIGHTH, dir);
				motorController->engage(2,SPEED::FIVE_EIGHTH, dir);
			}
		}
	}
*/
	motorController->stopNow();


}
void NavigationController::move(WALL_FOLLOWING following_mode, int distance, int angle, int forward_target_distance  ){

	this->updateLocation();
	while(1){
		if(following_mode == WALL_FOLLOWING::LEFT){
			if(this->currentLeftPos.minimumDistance < 300){
				//approaching wall
				motorController->engage(1,SPEED::HALF, MOVEMENT::FORWARD);
				motorController->engage(2,SPEED::QUARTER, MOVEMENT::FORWARD);
			}
			if(this->currentLeftPos.minimumDistance > 360){
				motorController->engage(1,SPEED::QUARTER, MOVEMENT::FORWARD);
				motorController->engage(2,SPEED::HALF, MOVEMENT::FORWARD);
			}
			if(this->currentFrontPos.averageDistance < 400){
				motorController->engage(1,SPEED::HALF, MOVEMENT::FORWARD);
				motorController->engage(2,SPEED::QUARTER, MOVEMENT::FORWARD);
			}

		}

		if(following_mode == WALL_FOLLOWING::RIGHT){

			if(this->currentRightPos.minimumDistance < 300){
				//approaching wall
				motorController->engage(2,SPEED::HALF, MOVEMENT::FORWARD);
				motorController->engage(1,SPEED::QUARTER, MOVEMENT::FORWARD);
			}
			if(this->currentRightPos.minimumDistance > 360){
				motorController->engage(2,SPEED::QUARTER, MOVEMENT::FORWARD);
				motorController->engage(1,SPEED::HALF, MOVEMENT::FORWARD);
			}
			if(this->currentFrontPos.averageDistance < 400){
				motorController->engage(2,SPEED::FULL, MOVEMENT::FORWARD);
				motorController->engage(1,SPEED::QUARTER, MOVEMENT::FORWARD);
			}
		}

	}

}

void NavigationController::turn(int targetAngle, double radius){

	this->updateLocation();
	while(this->currentFrontPos.minimumDistance < 300){
		small_turn();
		this->updateLocation();
	}

};

void NavigationController::small_turn(){
		motorController->engage(1,SPEED::THREE_EIGHTH, MOVEMENT::REVERSE);
		motorController->engage(2,SPEED::THREE_EIGHTH, MOVEMENT::FORWARD);
		usleep(400000);
		motorController->stop();
}

} /* namespace blaze */
