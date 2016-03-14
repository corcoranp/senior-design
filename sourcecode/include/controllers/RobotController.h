/*
 * RobotController.h
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
 *
 */

#ifndef INCLUDE_CONTROLLERS_ROBOTCONTROLLER_H_
#define INCLUDE_CONTROLLERS_ROBOTCONTROLLER_H_

#include <iostream>
#include <math.h>



#include "../system/indicator.h"
#include "../model/port.h"

/*
 * Reference the other controller classes
 */
#include "ArmController.h"
#include "ImagingController.h"
#include "MotorController.h"
#include "NavigationController.h"
#include "StorageController.h"

namespace blaze {
	/*
 	 * GLOBAL ROBOT PROPERTIES
 	 */
	extern int velocity;
	//extern int max_velocity =100;
	//extern int low_velocity = 10;

	class RobotController {
		public :

		static void *entry(void *arg);
		void start();							//Method for starting the Robot
		void reset();

		bool isSystemReady = false;
		bool isHardwareReady = false;
		bool isSensorsReady = false;
		bool isReady = false;

		indicator state_display;

		port currentPort;



		private:
			void systemCheck();
			void hardwareCheck();
			void sensorCheck();
			void readyStateCheck();

			MotorController *motorControl;
	};


}

#endif /* INCLUDE_CONTROLLERS_ROBOTCONTROLLER_H_ */
