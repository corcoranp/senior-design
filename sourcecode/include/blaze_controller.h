/*
 * blazebot
 * By: Peter Corcoran
 * Copyright: © 2015
 *
 * Jul 1, 2015 - Initial Coding
 *
 *
 *	Blazebot is the class encapsulation of the robot
 */

#ifndef INCLUDE_BLAZE_CONTROLLER_H_
#define INCLUDE_BLAZE_CONTROLLER_H_

/*
 * 1. define the robot manipulators
 * 2. define what behavior the robot has...
 */


#include <iostream>
#include <math.h>
#include <blazecore.h>


namespace blaze {
	/*
 	 * GLOBAL ROBOT PROPERTIES
 	 */
	extern int velocity;
	//extern int max_velocity =100;
	//extern int low_velocity = 10;


	class controller {
		public :



			static void *entry(void *arg);
			void start();



			bool isSystemReady = false;
			bool isHardwareReady = false;
			bool isSensorsReady = false;
			bool isReady = false;


			/*
			 * Behavior that allows the robot to move in some direction
			 *
			 * vector is the magnitude & direction in which the robot should travel.
			 * etime is the elapse time needed to slow down.
			 */
			void engage(); //void engage(vector, etime);

			void stop(); //void stop(vector, etime) = slow to stop

			void setPosition(Axis axis);
			void getPosition();

			//robot will only move in 2 directions x & y
			void setVelocity();


		private:
			void systemCheck();
			void hardwareCheck();
			void sensorCheck();
			void readyStateCheck();
	};


}






#endif /* INCLUDE_BLAZE_CONTROLLER_H_ */
