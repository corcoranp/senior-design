/*
 * robot.h
 *
 *  Created on: Jan 16, 2016
 *      Author: Peter
 */

#ifndef ROBOT_H_
#define ROBOT_H_

#include "storage.h"

using namespace std;

namespace blaze {

class robot {
	private:
		storage botStorage;

	public:

		storage getStorage();
		//properties are in mm
		int width = 300;
		int length = 300;

		int lidar_width = 77;
		int lidar_height = 82;
		//lidar position relative to the robot center
		//all robot distances will contain this offset when
		//read raw from the LIDAR system.
		//these values should be subtracted from distance measurements
		int lidar_x = -28;
		int lidar_y = 41;




};

}


#endif /* ROBOT_H_ */
