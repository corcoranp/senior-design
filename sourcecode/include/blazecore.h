/*
 * blaze_core.h
 *
 *	Main Variables of the system.
 *  Created on: Jul 20, 2015
 *      Author: Peter
 */

#ifndef INCLUDE_BLAZECORE_H_
#define INCLUDE_BLAZECORE_H_

#include "model/waypoint.h"

using namespace std;



static const string SYSTEM_SECTION 				= "SYSTEM";
static const string LOGGING_SECTION 			= "LOGGING";

static const string ROBOT_SECTION 				= "ROBOT";
static const string ARM_SECTION 				= "ARM";
static const string MOTORS_SECTION 				= "MOTORS";

//SENSORS ----------------------------------------------------------
static const string CAMERA_SECTION 				= "CAMRA";
static const string ACCELEROMETER_SECTION 		= "ACCELEROMETERS";
static const string ULTRASONIC_SECTION 			= "ULTRASONICS";
static const string IMAGING_SECTION 			= "IMAGING";

//MICROCONTROLLER --------------------------------------------------
static const std::string PINS_SECTION			= "PINS";

//ROBOT ROUTING   --------------------------------------------------

static const string NAMED_WAYPOINTS_SECTION 	= "NAMED_WAYPOINTS";
static const string PORT_COORDINATES_SECTION 	= "PORT_COORDINATES";
static const string ROUTES_SECTION 				= "ROUTES";


namespace blaze {
// DEFINES SYSTEM SETTINGS VARIABLES
	static int VERSION;
	static string NAME;
	static string CONTROL_MODE;
	static string START_MODE;

// DEFINES LOGGING SETTINGS VARIABLES
	static bool CONSOLE_ENABLED = true;
	static bool DEBUG_ENABLED;
	static string DEBUG_LEVEL;
	static bool LOGFILE_ENABLED;
	static string LOGFILE_NAME;
	static bool APPEND_TO_LOG;

// DEFINES USER SETTINGS


// DEFINES ROBOT PHYSICAL PROPERTIES

	static string ROBOT_X;
	static string ROBOT_Y;

// DEFINES ARM SETTINGS

	static bool ROBOT_ENABLED;

// DEFINES MOTOR SETTINGS
	static bool MOTORS_ENABLED;

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

	static waypoint *WP_TUNNEL_EXIT;

// PORT COORDINATE DEFINITIONS




// ROUTE DEFINITIIONS



// ENUMS

	enum Color {BLUE, GREEN, YELLOW, RED };
	enum Axis {x=1, y=2, z=3};




}

#endif /* INCLUDE_BLAZECORE_H_ */
