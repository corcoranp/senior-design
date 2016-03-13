
#include "../include/globals.h"

using namespace std;

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
	int gpio_stepper_dir = -1;
	int gpio_stepper_step = -1;
	int gpio_stepper_rst = -1;
	int gpio_stepper_ms3 = -1;
	int gpio_stepper_ms2 = -1;
	int gpio_stepper_m1 = -1;
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

	/*bool BASE_ENABLED;
	bool SHOULDER_ENABLED;
	bool ELBOW_ENABLED;
	bool WRIST_ENABLED;
	bool HAND_ROTAION_ENABLED;
	bool HAND_ENABLED;
	*/

	// DEFINES MOTOR SETTINGS
	bool MOTORS_ENABLED = false;
	int M1_MAX_SPEED = 0;
	int M2_MAX_SPEED = 0;

	// DEFNIES CAMERAS SETTINGS
	bool CAMERAS_ENABLED = false;
	string CAMERA_PORT = "";

	// DEFINES ACCELEROMETERS SETTINGS

	// DEFINES LIDAR SETTINGS
	bool LIDAR_ENABLED = false;
	string LIDAR_PORT = "";

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


