/*
 * Senior Design Project Bootloader
 * By: Peter Corcoran
 * Created: June 24, 2015
 *
 * Initial c++ coding for 2015-2016 UAB EE Senior Design
 *
 */

#include <ctime>
#include <stdio.h>

/*
 * BLAZEBOT INCLUDES
 */
#include "../include/botloader.h"
#include "../include/globals.h"
#include "../include/controllers/RobotController.h"

#include "../include/system/console.h"
#include "../include/system/SettingsReader.h"

#include "../include/model/qrcode.h"
#include "../include/model/zone.h"

#include "../include/io/dcmotor.h"

#include "../include/system/fileSerializer.h"



using namespace std;
using namespace blaze;



//================================================================================
// METHED IMPLEMENTATION
//================================================================================
/*
 * BlazeBot Bootloader Main Control
 *
 * A bootloader is a program that loads an operating system
 * when a computer is turned on, similarly the BlazeBot Bootloader
 * loads the robot's main system's operations, or sysops.
 *
 *
 * main is the main program's loop, sysops isn't loaded at start up
 * in case there is a reason to wait, or preloading of modules that
 * need to be done, then sysops.start() can be executed, and extended
 * to inject any system dependencies/settings.
 */
 int main(int argc, char* argv[])
{
	 console::print(console::currentDateTime());

	try
	{
		console::print("USAGE: ./blazebot -settings <path>");
		console::print("");
		console::print("STEP 1 - Read Command Arguments ");

		/*
		 * STEP 1: Read command arguments which should specify settings file...
		 */
		char * settingsFile = getCmdOption(argv, argv + argc, "-settings");
		if(!settingsFile){
			//sets default settings file
			settingsFile = "./resources/config.settings";
		}

		/*
		 * STEP 2: LOAD SETTINGS FILE
		 */
		console::print("STEP 2 - Load All Settings... ");
		SettingsReader reader(settingsFile);

		//check to see if the setting file was loaded correctly...
		if (reader.ParseError() < 0){
			console::print("Can't load 'config.settings, BLAZE cannot boot up. '");
			return 1; //exit if there is a problem...
		}

		loadAllSettings(reader);

		/*
		 * STEP 3: LOAD LOGGING STATE
		 */
		console::print("STEP 3 - Loading Logging State...");
		//console::setLogging(blaze::DEBUG_LEVEL, blaze::LOGFILE_NAME.c_str(),blaze::LOGFILE_ENABLED);
		setLogging();
		console::debug("- Switching to debug mode. Debug message - " );

		/*
		 * SHOW COOL BANNER
		 */
		showBanner();



	/*
	 * STEP 4: TEST CODE SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 */
		console::debug("STEP 4 - CUSTOM START UP CODE !!! ");

		 std::cout << "- Config loaded from 'config.settings': version="
		              << reader.GetInteger("system", "version", -1) << ", debug level="
					  << reader.Get("LOGGING", "DEBUG_LEVEL", "UNKNOWN") << ", name="
		              << reader.Get("user", "name", "UNKNOWN") << ", email="
		              << reader.Get("user", "email", "UNKNOWN") << ", pi=" << endl;






		 console::debug( "- Tunnel Exit " + reader.Get("NAMED_WAYPOINTS", "WP_TUNNEL_EXIT", "UNKNOWN"));
		 console::debug("To String of Tunnel WayPoint: " + blaze::PAWP_TUNNEL_EXIT->toString());



		/*
		 * STEP 5: CHECK START UP MODE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		 */
		 //SYSTEM>CONTROL_MODE
		 if(CONTROL_MODE == "CMD"){
			 console::debug("GOING INTO COMMAND MODE");
			 commandMode();
		 }

		/*
		 * STEP LAST: STARTS SYSTEM OPERATIONS
		 */
		console::debug("");
		console::debug("LAST STEP: Start Robot Controller - Do NOT expect console output");
		bootSystemOperations();
	}
	catch(const std::exception& e)
	{
		FILE_LOG(logERROR) << e.what();
	}

	//write log file out:
	EXIT_LOGGER(log_file);
    return 0;
}


/*
 * TODO: Throw away
 * 		 DCMotor dcm(new PWM("pwm_test_P9_42.12"), 116); //will export GPIO116
		   dcm.setDirection(DCMotor::ANTICLOCKWISE);
		   dcm.setSpeedPercent(50.0f);   //make it clear that a float is passed
		   dcm.go();
		   cout << "Rotating Anti-clockwise at 50% speed" << endl;
		   usleep(5000000);    //sleep for 5 seconds
		   dcm.reverseDirection();
		   cout << "Rotating clockwise at 50% speed" << endl;
		   usleep(5000000);
		   dcm.setSpeedPercent(100.0f);
		   cout << "Rotating clockwise at 100% speed" << endl;
		   usleep(5000000);
		   dcm.stop();
		   cout << "End of EBB DC Motor Example" << endl;
 */


 //================================================================================
 // Private Methods
 //================================================================================


 void commandMode(){
	 std::string s;

	 while(s != "exit"){
		 std::cout << "Enter string:" << flush;
		 std::getline(std::cin, s);
		 std::cout << "the string was: " << s << std::endl;
	 }
 }


 void setLogging(){
	 console::print("- Starting setLogging Method");

	/*
	 * SET LOGGING LEVELS by settings file...
	 */
	string logLevel 		= DEBUG_LEVEL;
	const char * strLogFile = LOGFILE_NAME.c_str();

	//Load from settings file or default to DEBUG1
	if(logLevel.length() >= 0){
		FILELog::ReportingLevel() = FILELog::FromString(logLevel);
	}

	cout << strLogFile << endl;
	//write to log
	if(blaze::LOGFILE_ENABLED){
		LOG_TO_FILE(log_file, strLogFile);
	}
	FILE_LOG(logDEBUG) << "Completed";
	console::debug("- Debug Configuration Completed: " + DEBUG_LEVEL);
 }


/*
 * bootSystemOperations
 * Function spins off a thread to start the system operations object...
 */
void bootSystemOperations(){
	console::debug("- Starting System Operation Thread");
	//STEP 1: Define Handle to the Thread:
	pthread_t controllerEntry;
	//OPTIONAL: Define the data received back from pthread.
	void* result;
	//STEP 2: Create thread, pass reference, addr of the function and data

	 if(pthread_create(&controllerEntry, NULL, &RobotController::entry, NULL)){
		FILE_LOG(logERROR) << "Failed to create the thread, exiting.";
		return;
	}
	//STEP 3: Allow the Thread to complete...
	console::debug("- Allow Robot Controller Thread to Complete.");
	pthread_join (controllerEntry, &result);

	return;
}



//================================================================================
// HELPER METHODS
//================================================================================

 // Get Command Line Options
 char* getCmdOption(char ** begin, char ** end, const std::string & option)
 {
     char ** itr = std::find(begin, end, option);
     if (itr != end && ++itr != end)
     {
         return *itr;
     }
     return 0;
 }

 // Check to see if command line options exists
 bool cmdOptionExists(char** begin, char** end, const std::string& option)
 {
     return std::find(begin, end, option) != end;
 }




 void loadAllSettings(SettingsReader &reader){

	 //SET [SYSTEM] SETTINGS
	 blaze::VERSION 		= reader.GetInteger("system", "version", -1);
	 blaze::NAME 			= reader.Get("system", "name", "BLAZE");
	 blaze::CONTROL_MODE 	= reader.Get("system", "control_mode", "CMD");
	 blaze::START_MODE		= reader.Get("system", "start_mode", "START");


	 //SET LOGGING VARIABLES ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 blaze::CONSOLE_ENABLED		= reader.GetBoolean("logging", "console_enabled", true);
	 blaze::DEBUG_ENABLED 		= reader.GetBoolean("logging", "DEBUG_ENABLED", true);
	 blaze::DEBUG_LEVEL 		= reader.Get("logging", "DEBUG_LEVEL", "DEBUG");
	 blaze::LOGFILE_ENABLED 	= reader.GetBoolean("logging", "LOGFILE_ENABLED", true);
	 blaze::LOGFILE_NAME 		= reader.Get("logging", "LOGFILE_NAME", "./blaze_debug.log");
	 blaze::APPEND_TO_LOG 		= reader.GetBoolean("logging", "APPEND_TO_LOG", false);

	 //Set [ROBOT] Physical settings

	 blaze::ROBOT_X 			= reader.GetInteger("robot", "robot_x", 12);
	 blaze::ROBOT_Y 			= reader.GetInteger("robot", "robot_y", 12);
	 blaze::ROBOT_Z 			= reader.GetInteger("robot", "robot_z", 12);

	 //USER SETTINGS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


	 // HARDWARE CONTROL SETTINGS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 //	ARM
	 ARM_ENABLED 				= reader.GetBoolean("arm", "arm_enabled", true);


	 // MOTORS

	 MOTORS_ENABLED				= reader.GetBoolean("motors", "motors_enabled", true);
	 M1_MAX_SPEED				= reader.GetInteger("motors", "m1_max_speed", true);
	 M2_MAX_SPEED				= reader.GetInteger("motors", "m2_max_speed", true);

	 // CAMERAS
	 CAMERAS_ENABLED			= reader.GetBoolean("cameras", "camera_enabled", true);

	 // LIDAR
	 LIDAR_ENABLED				= reader.GetBoolean("lidar", "lidar_enabled", true);

	 // PINS
	 PINS_ENABLED				= reader.GetBoolean("pins", "pins_enabled", true);

	 // NAMED WAYPOINTS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 string pawp_tunnel_exit 	= reader.Get("named_waypoints", "pawp_tunnel_exit", "");
	 PAWP_TUNNEL_EXIT 			= new waypoint(pawp_tunnel_exit);

	 string pbwp_tunnel_exit 	= reader.Get("named_waypoints", "pbwp_tunnel_exit", "");
	 PBWP_TUNNEL_EXIT 			= new waypoint(pbwp_tunnel_exit);

	 // PORT COORDINATE DEF ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 // 		= reader.Get("named_waypoint", "wp_tunnel_exit", "");


	 // ROUTE DEFINITIONS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 // 		= reader.Get("named_waypoint", "wp_tunnel_exit", "");

	 // PORT SETTINGS
	 PA_ZONEA_INVENTORY 		= reader.Get("PORT_SETTINGS", "pa_zonea_inventory", "");
	 console::debug(PA_ZONEA_INVENTORY);
	 PA_ZONEB_INVENTORY 		= reader.Get("PORT_SETTINGS", "pa_zoneb_inventory", "");
	 console::debug(PA_ZONEB_INVENTORY);
	 PA_ZONEC_INVENTORY 		= reader.Get("PORT_SETTINGS", "pa_zonec_inventory", "");
	 console::debug(PA_ZONEC_INVENTORY);
	 PB_ZONEA_INVENTORY 		= reader.Get("PORT_SETTINGS", "pb_zonea_inventory", "");
	 console::debug(PB_ZONEA_INVENTORY);
	 PB_ZONEB_INVENTORY 		= reader.Get("PORT_SETTINGS", "pb_zoneb_inventory", "");
	 console::debug(PB_ZONEB_INVENTORY);
	 PB_ZONEC_INVENTORY 		= reader.Get("PORT_SETTINGS", "pb_zonec_inventory", "");
	 console::debug(PB_ZONEC_INVENTORY);
 }



 /*
  * Generic Method for printing text to the console
  */
  /*void print (string msg){
 	 cout << msg << endl;
  }
  void debug (string msg){
 	if(blaze::LOGFILE_ENABLED){
 		FILE_LOG(logDEBUG) << msg;
 	}
 	if(blaze::DEBUG_ENABLED){
 		print(msg);
 	}
  }
  void info (string msg){
 	if(blaze::LOGFILE_ENABLED){
 		FILE_LOG(logINFO) << msg;
 	}
 	if(blaze::DEBUG_ENABLED){
 		print(msg);
 	}
  }*/




/*
 * BLAZE BOT BANNER TEXT
 */
 void showBanner(){
	 console::info("                     _                    _");
	 console::info("                  ,/                        \\,");
	 console::info("        _________{(                          })_________");
	 console::info("       /.-------./\\                        //\\.-------.\\");
	 console::info("      //@@@@@@@//@@\\\\  )                (  //@@\\\\@@@@@@@\\\\");
	 console::info("     //@@@@@@@//@@@@>>/                  \\<<@@@@\\\\@@@@@@@\\\\");
	 console::info("    //O@O@O@O//@O@O//                      \\\\O@O@\\\\O@O@O@O\\\\");
	 console::info("  //OOOOOOOO//OOOO||          \\  /          ||OOOO\\\\OOOOOOOO\\\\");
	 console::info(" //O%O%O%O%//O%O%O%\\\\         ))((         //%O%O%O\\\\%O%O%O%O\\\\");
	 console::info("||%%%%%%%%//'  `%%%%\\\\       //  \\       //%%%%'   `\\\\%%%%%%%||");
	 console::info("((%%%%%%%((      %%%%%\\\\    ((    ))    //%%%%%       ))%%%%%%))");
	 console::info(" \\\\:::' `::\\      `:::::\\\\   \\)~~(/    //:::::'      //::' `:::/");
	 console::info("  )'     `;)'      (`  ` \\\\ `<@  @>' / / '  ')      `(;'     `(");
	 console::info("          (               \\`\\ )^^( /  /               )");
	 console::info("        _                  ) \\\\oo/   (");
	 console::info("       (@)                  \\  `'   /                      _");
	 console::info("       |-|\\__________________\\__^__<________oOo__________ (@)");
	 console::info("       |-|                                  VVV          \\|-|");
	 console::info("       |-|             2016 IEEE SOUTHEASTCON             |-|");
	 console::info("       |-|           UAB ELECTRICAL ENGINEERING           |-|");
	 console::info("       |-|                                                |-|");
	 console::info("       |-|           ~~~~~~~~~~ BLAZE ~~~~~~~~~           |-|");
	 console::info("       |-|                                                |-|");
	 console::info("       |-|\\_____________________________________________  |-|");
	 console::info("       (@)                 / ,/ \\_____/ \\ ~\\/~         `\\|-|");
	 console::info("        ~             ___//^~      \\____/\\               (@)");
	 console::info("                     <<<  \\     __  <____/||               ~");
	 console::info("                               <   \\ <___/||");
	 console::info("                                  || <___//");
	 console::info("                                  \\ \\/__//");
	 console::info("                                   ~----~");
	 console::info("");
	 console::info("");
	 console::info("Starting Birmingham's Logistics Actuating Zone Evaluator ...");
	 return;
 }
