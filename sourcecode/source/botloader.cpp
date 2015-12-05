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
#include "../include/blazecore.h"
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
		 console::debug("To String of Tunnel WayPoint: " + blaze::WP_TUNNEL_EXIT->toString());



		/*
		 * STEP 5: CHECK START UP MODE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		 */
		 //SYSTEM>CONTROL_MODE
		 if(reader.Get("SYSTEM", "CONTROL_MODE", "UNKNOWN") == "CMD"){
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
	string logLevel 		= blaze::DEBUG_LEVEL;
	const char * strLogFile = blaze::LOGFILE_NAME.c_str();

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
	console::debug("- Debug Configuration Completed: " + blaze::DEBUG_LEVEL);
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

	 //SET SYSTEM SETTINGS
	 blaze::VERSION = reader.GetInteger("system", "version", -1);
	 blaze::NAME 		= reader.Get("system", "name", "BLAZE");

	 //SET LOGGING VARIABLES ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 blaze::DEBUG_ENABLED 		= reader.GetBoolean(LOGGING_SECTION, "DEBUG_ENABLED", true);
	 blaze::DEBUG_LEVEL 		= reader.Get(LOGGING_SECTION, "DEBUG_LEVEL", "DEBUG");
	 blaze::LOGFILE_ENABLED 	= reader.GetBoolean(LOGGING_SECTION, "LOGFILE_ENABLED", true);
	 blaze::LOGFILE_NAME 		= reader.Get(LOGGING_SECTION, "LOGFILE_NAME", "./blaze_debug.log");
	 blaze::APPEND_TO_LOG 		= reader.GetBoolean(LOGGING_SECTION, "APPEND_TO_LOG", false);



	 //USER SETTINGS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


	 // HARDWARE CONTROL SETTINGS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 //	ARM

	 // MOTORS

	 // CAMERAS

	 // LIDAR

	 // PINS


	 // NAMED WAYPOINTS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 string wp = reader.Get("named_waypoints", "wp_tunnel_exit", "");
	 //blaze::WP_TUNNEL_EXIT = new waypoint(0.0, 0.0, 0.0);
	 blaze::WP_TUNNEL_EXIT = new waypoint(wp);


	 // PORT COORDINATE DEF ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 // 		= reader.Get("named_waypoint", "wp_tunnel_exit", "");


	 // ROUTE DEFINITIONS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 // 		= reader.Get("named_waypoint", "wp_tunnel_exit", "");

 }





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
