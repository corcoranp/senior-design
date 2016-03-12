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



	//. Identify Port @ startup
	//. Navigate through tunnel
	//. Solve section A
	//. Solve section C
	//. Solve section B

	console::debug("RobotController Ended");
}



