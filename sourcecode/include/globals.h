/*
 * globals.h
 *
 *  Created on: Jan 16, 2016
 *      Author: Peter
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include "model/waypoint.h"

using namespace std;

namespace blaze {


	/*
	 * Callback functions are implemented in C++ using function pointers
	 * Function pointers are pointers that store the address of a function.
	 * It is possible to pass these pointers to other functions, which can
	 * dereference the function pointer and call the function that is passed.
	 *
	 *	These are useful when trying to chain functions together.
	 *
	 * to use:
	 * ...
	 *  	int doSomething(CallbackType callback){
	 *  		return callback(10); //execute callback function with passed value 10
	 *  	}
	 *
	 *  	void callbackFunction(int var) {
	 *  		...
	 *  		do something useful
	 *  		...
	 *  	}
	 *
	 * 	int main(){
	 * 		...
	 * 		int y = doSomething(&callbackFunction);
	 * 		...
	 * 	}
	 *
	 */
	typedef int (*CallbackType) (int);


// DEFINES [SYSTEM] SETTINGS VARIABLES
	static int VERSION = 0;
	static string NAME;
	static string CONTROL_MODE;
	static string START_MODE;

// DEFINES [LOGGING] SETTINGS VARIABLES
	static bool CONSOLE_ENABLED = true;
	static bool DEBUG_ENABLED;
	static string DEBUG_LEVEL;
	static bool LOGFILE_ENABLED;
	static string LOGFILE_NAME;
	static bool APPEND_TO_LOG;

// DEFINES USER SETTINGS


// DEFINES [ROBOT] PHYSICAL PROPERTIES

	static string ROBOT_X;
	static string ROBOT_Y;
	static string ROBOT_Z;

// DEFINES [ARM] SETTINGS

	static bool ARM_ENABLED;
	static bool BASE_ENABLED;
	static bool SHOULDER_ENABLED;
	static bool ELBOW_ENABLED;
	static bool WRIST_ENABLED;
	static bool HAND_ROTAION_ENABLED;
	static bool HAND_ENABLED;


// DEFINES MOTOR SETTINGS
	static bool MOTORS_ENABLED;
	static int M1_MAX_SPEED;
	static int M2_MAX_SPEED;

// DEFNIES CAMERAS SETTINGS
	static bool CAMERAS_ENABLED;

// DEFINES ACCELEROMETERS SETTINGS


// DEFINES ULTRASONICS SETTINGS

// DEFINES LIDAR SETTINGS
	static bool LIDAR_ENABLED;

// MICROCONTROLLER SETTINGS
// DEFINES PINS

	static bool PINS_ENABLED;

// NAMED WAYPOINTS SETTINGS

	static waypoint *PAWP_TUNNEL_EXIT;
	static waypoint *PBWP_TUNNEL_EXIT;


// PORT COORDINATE DEFINITIONS




// ROUTE DEFINITIIONS


	// PORT SETTINGS
	static string PA_ZONEA_INVENTORY;
	static string PA_ZONEB_INVENTORY;
	static string PA_ZONEC_INVENTORY;
	static string PB_ZONEA_INVENTORY;
	static string PB_ZONEB_INVENTORY;
	static string PB_ZONEC_INVENTORY;

}



#endif /* GLOBALS_H_ */
