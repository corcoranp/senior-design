/*
 * sysops - System Operations
 * By: Peter Corcoran
 * Copyright: © 2015
 *
 * Jun 27, 2015 - Initial Coding
 *
 * This file is used as the main controller system of
 *
 */
#include <iostream>
#include "../include/sysops.h"

using namespace std;

void *sysops::entry(void *args){
	sysops *thisCls = (sysops *)args;
	thisCls-> start();
}

void sysops::start(){


	cout << "entered start of sysops" << endl;
}
