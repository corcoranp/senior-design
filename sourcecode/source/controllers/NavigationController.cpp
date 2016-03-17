/*
 * NavigationController.cpp
 *
 *  Created on: Mar 12, 2016
 *      Author: Peter
 */

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include "../../include/controllers/NavigationController.h"

#include "../../include/enums.h"
#include "../../include/globals.h"
#include "../include/system/console.h"
#include "../../include/controllers/MotorController.h"

using namespace std;




int angle_in_radians = 0; // Change this value from 0 to output the angle in radians

namespace blaze {

/*
 * Constructor
 */
NavigationController::NavigationController(){
	this->init();
}

/*
 * Constructor
 */
NavigationController::NavigationController(MotorController *mc) {
	motorController = mc;
	this->init();
}

void NavigationController::init(){

	lidar = lidarIO(LIDAR_PORT);
	lidar.connect();

	this->currentFrontPos.MaximumAngle = FRONT_MAX;

}

NavigationController::~NavigationController() {
	// TODO Auto-generated destructor stub
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

		lidarIO lidar(LIDAR_PORT);
		lidar.connect();
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



/* Values have been adjusted to set due north of the bot to 90 degrees */
/* Back values added with 90 degrees accounted for                     */
double NavigationController::getPosition (Face f, DistType dt){

	console::debug("LIDAR: get Data");
	lidar.getData(lidar.lidarFileDescriptor, data);

	/*
	calculateAverage(currentFrontPos);
	calculateMinimum(currentFrontPos);
	calculateMaximum(currentFrontPos);
	calculateTheta(currentFrontPos);
	*/

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
 * Used to calculate the Average distance in position object
 */
void NavigationController::calculateAverage(angleRange *p){
	int index;
	double sum;
	int count;


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

void NavigationController::alignToFace(Face f){
	console::debug("NC: alignToFace");
	double t = this->getPosition(f, DistType::Theta);


	//while the absolute value of theta is greater than one...
	while(abs(t) >5){
		if(t>1){
			console::debug("NC: Angle is positive, needs counter-clockwise alignment : " + to_string(abs(t)));
			//counter-clock
			motorController->engage(1,SPEED::QUARTER, MOVEMENT::FORWARD);
			motorController->engage(2,SPEED::QUARTER, MOVEMENT::REVERSE);
		}
		if(t < -1){
			//clockwise
			console::debug("NC: Angle is negative, needs clockwise alignment : " + to_string(abs(t)));
			motorController->engage(2,SPEED::QUARTER, MOVEMENT::FORWARD);
			motorController->engage(1,SPEED::QUARTER, MOVEMENT::REVERSE);
		}

		t = this->getPosition(f, DistType::Theta);

		motorController->stopNow();
		usleep(50000);

	}

	motorController->stopNow();


}

void NavigationController::moveToFirstPosition(){}

void NavigationController::move(WALL_FOLLOWING following_mode, int distance, int angle, int forward_target_distance  ){



}



} /* namespace blaze */
