/*
 * RobotController.cpp
 *
 * By: Peter Corcoran
 * Copyright: © 2015
 *
 * Jun 27, 2015 - Initial Coding
 *
 * This file is used as the main controller system of
 * The responsibility of this class is to control system operations
 *
 * 1. System Check
 * 2. Hardware Check
 * 3. Sensor Check
 * 4. Boot State Check
 * 5. Running each port delivery
 *
 */
#include <iostream>
#include <map>
#include <string>

#include "../../include/system/console.h"

#include "../../include/controllers/RobotController.h"
#include "../include/controllers/NavigationController.h"
#include "../include/controllers/StorageController.h"


using namespace std;
using namespace blaze;

/*
 * Thread function that will execute when the thread is created
 * it passes and receives data by void pointers
 */
void *RobotController::entry(void *args){
	RobotController *thisClass = (RobotController *)args; //cast the data passed to pointer to sysops
	thisClass->start();
}

/*
 * Start function runs at boot to handle check events....
 */
void RobotController::start(){
	console::debug("RobotController Started");
	//Thread that runs the main robot functions.

	console::print("Hello, My name is BLAZE...let me take you on a jouney...");

	//StorageController sc(STORAGE_PWM);
	//sc.setRestPosition();

	MotorController mc(M1_PWM, M2_PWM, gpio_motor1_dir, gpio_motor2_dir);


	console::debug("motor controller created");
	mc.forward(SPEED::SEVEN_EIGHTH);
	while(mc.isBusy()){
		usleep(10000);
	}

	usleep (2000000);


	mc.forward(SPEED::QUARTER);
		while(mc.isBusy()){
			usleep(10000);
		}
		usleep (4000000);

	mc.stop();

	while(mc.isBusy()){
		usleep(10000);
	}


	/*
	this->motorControl = new MotorController(M1_PWM, M2_PWM, gpio_motor1_dir, gpio_motor2_dir);

	console::debug("motor controller created");
	this->motorControl->forward();

	usleep(1000000);

	this->motorControl->stop();
*/
	//. Identify Port @ startup
/*
	this->currentPort.portcfg = NavigationController::determinePort();
	if(this->currentPort.portcfg == PortConfig::A){

		console::info("Port A Detected");
	}
	if(this->currentPort.portcfg == PortConfig::B){

		console::info("Port B Detected");
	}
	if(this->currentPort.portcfg == PortConfig::UNDEFINED){
		console::error("Port Undetected");
		state_display.errorState();
	}

*/
	//. Navigate through tunnel





	//. Solve section A
	//. Solve section C
	//. Solve section B

	 /*if(!LIDAR_ENABLED){
		 char *port = new char[ LIDAR_PORT.length() + 1];
		 strcpy(port,  LIDAR_PORT.c_str());
		 lidarIO lidar(port);
		 lidar.disable();
	 }*/


	console::debug("RobotController Ended");
}



