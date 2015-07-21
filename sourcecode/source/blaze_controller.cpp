/*
 * sysops - System Operations
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
 *
 */
#include <iostream>
#include <map>
#include <string>
#include "blaze_controller.h"

#ifdef LOG1
#include "system/log1.h"
#else
#include "system/log.h"
#endif

using namespace std;
using namespace blaze;

//Thread function that will execute when the thread is created
//it passes and receives data by void pointers
void *controller::entry(void *args){
	controller *thisClass = (controller *)args; //cast the data passed to pointer to sysops
	thisClass->start();
}

/*
 * Start function runs at boot to handle check events....
 */
void controller::start(){
	FILE_LOG(logDEBUG)  << "SysOps Started";

	cout << "SysOps Ended" << endl;
}


