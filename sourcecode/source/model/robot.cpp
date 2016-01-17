/*
 * robot.cpp
 *
 *  Created on: Jan 16, 2016
 *      Author: Peter
 */

#include "../../include/model/robot.h"

using namespace std;
using namespace blaze;


/*
 * Function returns the storage object of the robot
 */
storage robot::getStorage(){
	return robot::botStorage;
}

