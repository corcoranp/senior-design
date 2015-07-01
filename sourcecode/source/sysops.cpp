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
 */
#include <iostream>
#include "../include/sysops.h"

#ifdef LOG1
#include "log1.h"
#else
#include "log.h"
#endif

using namespace std;


//Thread function that will execute when the thread is created
//it passes and receives data by void pointers
void *sysops::entry(void *args){
	sysops *thisClass = (sysops *)args; //cast the data passed to pointer to sysops
	thisClass->start();
}


void sysops::start(){
	FILE_LOG(logDEBUG)  << "SysOps Started";

	cout << "SysOps Ended" << endl;
}
