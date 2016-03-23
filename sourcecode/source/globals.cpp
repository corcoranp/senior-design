
#include "../include/globals.h"
#include "../include/enums.h"
#include <cmath>

using namespace std;
using namespace blaze;

	unsigned int ZERO = 0;
// DEFINES [SYSTEM] SETTINGS VARIABLES
	int VERSION = 0;
	string NAME;
	string CONTROL_MODE;
	string START_MODE;

	// DEFINES [LOGGING] SETTINGS VARIABLES
	bool CONSOLE_ENABLED = true;
	bool DEBUG_ENABLED;
	string DEBUG_LEVEL;
	bool LOGFILE_ENABLED;
	string LOGFILE_NAME;
	bool APPEND_TO_LOG;

	// DEFINES USER SETTINGS


	// DEFINES [ROBOT] PHYSICAL PROPERTIES

	string ROBOT_X;
	string ROBOT_Y;
	string ROBOT_Z;

	int gpio_start_switch = -1;
	int gpio_stop_switch = -1;
	int gpio_stepper_sleep = -1;
	int gpio_stepper_dir = -1;
	int gpio_stepper_step = -1;
	int gpio_stepper_rst = -1;
	int gpio_stepper_ms3 = -1;
	int gpio_stepper_ms2 = -1;
	int gpio_stepper_ms1 = -1;
	int gpio_stepper_en = -1;
	int gpio_stepper_lsw = -1;
	int gpio_stepper_rsw = -1;
	int gpio_led1 = -1;
	int gpio_led2 = -1;
	int gpio_led3 = -1;
	int gpio_led4 = -1;
	int gpio_motor1_dir = -1;
	int gpio_motor2_dir = -1;
	int gpio_motor1_pwm = -1;
	int gpio_motor2_pwm = -1;
	int gpio_linear_pwm = -1;

	// DEFINES [ARM] SETTINGS

	bool ARM_ENABLED = false;
	string ARM_PORT = "";
	int ARM_SHOULDER_ID = 20;
	int ARM_ELBOW1_ID = 21;
	int ARM_ELBOW2_ID = 22;
	int ARM_ELBOW3_ID = 23;
	int ARM_WRIST_ID = 24;
	int ARM_HAND_ID = 25;

	/*bool BASE_ENABLED;
	bool SHOULDER_ENABLED;
	bool ELBOW_ENABLED;
	bool WRIST_ENABLED;
	bool HAND_ROTAION_ENABLED;
	bool HAND_ENABLED;
	*/

	// DEFINES MOTOR SETTINGS
	bool MOTORS_ENABLED = false;
	int MOTOR_PWM_PERIOD = 0;
	int MOTOR_MAX_DUTY = 0;
	int M1_MAX_SPEED = 0;
	int M2_MAX_SPEED = 0;
	string M1_PWM = "";
	string M2_PWM = "";

	// DEFNIES CAMERAS SETTINGS
	bool CAMERAS_ENABLED = false;
	string CAMERA_PORT = "";

	// DEFINES ACCELEROMETERS SETTINGS

	// DEFINES LIDAR SETTINGS
	bool LIDAR_ENABLED = false;
	string LIDAR_PORT = "";
	int FRONT_MAX = -1;
	int FRONT_MIN = -1;
	int RIGHT_MAX = -1;
	int RIGHT_MIN = -1;
	int LEFT_MAX = -1;
	int LEFT_MIN = -1;
	int BACK_MAX = -1;
	int BACK_MIN = -1;



	string STORAGE_PWM = "";


	// MICROCONTROLLER SETTINGS
	// DEFINES PINS

	bool PINS_ENABLED = false;

	// NAMED WAYPOINTS SETTINGS

	blaze::waypoint *PAWP_TUNNEL_EXIT;
	blaze::waypoint *PBWP_TUNNEL_EXIT;


	// PORT COORDINATE DEFINITIONS




	// ROUTE DEFINITIIONS


	// PORT SETTINGS
	string PA_ZONEA_INVENTORY = "";
	string PA_ZONEB_INVENTORY = "";
	string PA_ZONEC_INVENTORY = "";
	string PB_ZONEA_INVENTORY = "";
	string PB_ZONEB_INVENTORY = "";
	string PB_ZONEC_INVENTORY = "";

char* blaze::string2char (string s){
	char *retval = new char[ s.length() + 1];
	strcpy(retval,  s.c_str());
	return retval;
}

bool isValueEqual(double a, double b, double acc) {
	return abs(a - b) < acc;
}

QUADRANT inQuadrant(int angle){
	QUADRANT q;
	if(angle >= 0 && angle < 90){ q = QUADRANT::I; }
	if(angle >= 90 && angle < 180){ q = QUADRANT::II; }
	if(angle >= 180 && angle < 270){ q = QUADRANT::III; }
	if(angle >= 270 && angle < 360){ q = QUADRANT::IV; }

	return q;
}
