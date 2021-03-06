/*
 * globals.h
 *
 *  Created on: Jan 16, 2016
 *      Author: Peter
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <mutex>
#include <math.h>

#include "model/waypoint.h"

using namespace std;
#ifndef ANG_TO_RAD
#define ANG_TO_RAD(angle) ((angle*M_PI)/180.0)
#endif

#ifndef RAD_TO_ANG
#define RAD_TO_ANG(x) ((x*180.0)/M_PI)
#endif

#ifndef uint8_t
typedef unsigned char uint8_t;
#endif

extern bool isValueEqual(double a, double b, double acc);

	extern double dist[365];
	extern mutex bot_mutex;

	extern unsigned int ZERO;
// DEFINES [SYSTEM] SETTINGS VARIABLES
	extern int VERSION;
	extern string NAME;
	extern string CONTROL_MODE;
	extern string START_MODE;
	extern bool AUTO_RESTART;

	// DEFINES [LOGGING] SETTINGS VARIABLES
	extern bool CONSOLE_ENABLED;
	extern bool DEBUG_ENABLED;
	extern string DEBUG_LEVEL;
	extern bool LOGFILE_ENABLED;
	extern string LOGFILE_NAME;
	extern bool APPEND_TO_LOG;

	// DEFINES USER SETTINGS


	// DEFINES [ROBOT] PHYSICAL PROPERTIES
	extern string ROBOT_X;
	extern string ROBOT_Y;
	extern string ROBOT_Z;

	extern int gpio_start_switch;
	extern int gpio_stop_switch;
	extern int gpio_stepper_sleep;
	extern int gpio_stepper_dir;
	extern int gpio_stepper_step;
	extern int gpio_stepper_rst;
	extern int gpio_stepper_ms3;
	extern int gpio_stepper_ms2;
	extern int gpio_stepper_ms1;
	extern int gpio_stepper_en;
	extern int gpio_stepper_lsw;
	extern int gpio_stepper_rsw;
	extern int gpio_led1;
	extern int gpio_led2;
	extern int gpio_led3;
	extern int gpio_led4;
	extern int gpio_motor1_dir;
	extern int gpio_motor2_dir;
	extern int gpio_motor1_pwm;
	extern int gpio_motor2_pwm;
	extern int gpio_linear_pwm;

	// DEFINES [ARM] SETTINGS

	extern bool ARM_ENABLED;
	extern string ARM_PORT;
	extern int ARM_SHOULDER_ID;
	extern int ARM_ELBOW1_ID;
	extern int ARM_ELBOW2_ID;
	extern int ARM_ELBOW3_ID;
	extern int ARM_WRIST_ID;
	extern int ARM_HAND_ID;

	/*extern bool BASE_ENABLED;
	extern bool SHOULDER_ENABLED;
	extern bool ELBOW_ENABLED;
	extern bool WRIST_ENABLED;
	extern bool HAND_ROTAION_ENABLED;
	extern bool HAND_ENABLED;
	*/

	// DEFINES MOTOR SETTINGS
	extern bool MOTORS_ENABLED;
	extern int MOTOR_PWM_PERIOD;
	extern int MOTOR_MAX_DUTY;
	extern int M1_MAX_SPEED;
	extern int M2_MAX_SPEED;
	extern string M1_PWM;
	extern string M2_PWM;

	// DEFNIES CAMERAS SETTINGS
	extern bool CAMERAS_ENABLED;
	extern string CAMERA_PORT;

	// DEFINES ACCELEROMETERS SETTINGS

	// DEFINES LIDAR SETTINGS
	extern bool LIDAR_ENABLED;
	extern bool SCANNING;
	extern bool QUEUING_ENABLED;
	extern string LIDAR_PORT;
	extern int ZERO_REF;
	extern int FRONT_MAX;
	extern int FRONT_MIN;
	extern int BACK_MAX;
	extern int BACK_MIN;
	extern int RIGHT_MAX;
	extern int RIGHT_MIN;
	extern int LEFT_MAX;
	extern int LEFT_MIN;

	extern string STORAGE_PWM;
	extern int LOAD_POSITION;
	extern int DRIVE_POSITION;
	extern int DELIVERY_POSITION;

	// MICROCONTROLLER SETTINGS
	// DEFINES PINS

	extern bool PINS_ENABLED;

	// NAMED WAYPOINTS SETTINGS

	extern blaze::waypoint *PAWP_TUNNEL_EXIT;
	extern blaze::waypoint *PBWP_TUNNEL_EXIT;


	// PORT COORDINATE DEFINITIONS


	// ROUTE DEFINITIIONS


	// PORT SETTINGS
	extern string PA_ZONEA_INVENTORY;
	extern string PA_ZONEB_INVENTORY;
	extern string PA_ZONEC_INVENTORY;
	extern string PB_ZONEA_INVENTORY;
	extern string PB_ZONEB_INVENTORY;
	extern string PB_ZONEC_INVENTORY;



namespace blaze {

	char* string2char (string s);


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


	void rest(int useconds);
	bool NOT(bool b);
	bool OR(bool b1, bool b2);
	bool AND(bool b1, bool b2);
}




#endif /* GLOBALS_H_ */
