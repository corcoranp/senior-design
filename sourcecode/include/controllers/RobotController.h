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
#include "../system/starter.h"
#include "../system/indicator.h"
#include "../model/port.h"

/*
 * Reference the other controller classes
 */

#include "ArmController.h"
#include "ImagingController.h"
#include "MotorController.h"
#include "StorageController.h"
#include "NavigationController.h"
#include "../io/StepperMotor.h"

namespace blaze {

	/*
 	 * GLOBAL ROBOT PROPERTIES
 	 */
	extern int velocity;

	class RobotController {
		public :

		//RobotController();
		//virtual ~RobotController();

		static void *entry(void *arg);
		static bool getIsRunning();
		static void setIsRunning(bool val);
		void start();							//Method for starting the Robot
		void reset();

		static bool hasPortBeenFound;
		static bool m_isRunning;


		static starter *control_buttons;
		static indicator *state_display;
		static MotorController *motorControl;
		static StepperMotor *stepperControl;
		static ArmController *armControl;
		static StorageController *storageControl;
		static ImagingController *imageControl;
		static NavigationController *navControl;


		static PortConfig currentPortConfig;
		static workqueue<measurement*>* m_queue;


		private:
			void systemCheck();
			void hardwareCheck();
			void sensorCheck();
			void readyStateCheck();

			static bool solveTunnel();
			static bool solveZoneA();
			static bool solveZoneB();
			static bool solveZoneC();
			static bool scanRail();
			static bool dance();


	};


}

#endif /* INCLUDE_CONTROLLERS_ROBOTCONTROLLER_H_ */
