/*
 * blaze_core.h
 *
 *  Created on: Jul 20, 2015
 *      Author: Peter
 */

#ifndef INCLUDE_BLAZECORE_H_
#define INCLUDE_BLAZECORE_H_

static const std::string SYSTEM_SECTION 		= "SYSTEM";
static const std::string LOGGING_SECTION 		= "LOGGING";
static const std::string ARM_SECTION 			= "ARM";
static const std::string CAMERA_SECTION 		= "CAMERAS";
static const std::string ACCELEROMETER_SECTION 	= "ACCELEROMETERS";
static const std::string ULTRASONIC_SECTION 	= "ULTRASONICS";
static const std::string PINS_SECTION			= "PINS";

namespace blaze {
// DEFINES SYSTEM SETTINGS VARIABLES
	static int VERSION;
	static std::string NAME;
	static std::string CONTROL_MODE;
	static std::string START_MODE;

// DEFINES LOGGING SETTINGS VARIABLES
	static bool DEBUG_ENABLED;
	static std::string DEBUG_LEVEL;
	static bool LOGFILE_ENABLED;
	static std::string LOGFILE_NAME;
	static bool APPEND_TO_LOG;


	enum Color {BLUE, GREEN, YELLOW, RED };
	enum Axis {x=1, y=2, z=3};




}

#endif /* INCLUDE_BLAZECORE_H_ */
