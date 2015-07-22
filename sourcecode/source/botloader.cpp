/*
 * Senior Design Project Bootloader
 * By: Peter Corcoran
 * Created: June 24, 2015
 *
 * Initial c++ coding for 2015-2016 UAB EE Senior Design
 *
 */

/*
 * BLAZEBOT INCLUDES
 */
#include "botloader.h"
#include "blazecore.h"
#include "blaze_controller.h"
#include "system/SettingsReader.h"

#include "model/qrcode.h"
#include "model/zone.h"



using namespace std;
using namespace blaze;

void print(string);
void debug(string);
void info(string);



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
	try
	{
		 cout << "blazebot starting" << endl;
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
		SettingsReader reader(settingsFile);

		//check to see if the setting file was loaded correctly...
		if (reader.ParseError() < 0){
			std::cout << "Can't load 'config.settings, BLAZE cannot boot up. '\n";
			return 1; //exit if there is a problem...
		}

		/*
		 * STEP 3: LOAD LOGGING STATE
		 */
		print("About to start setLogging");
		setLogging(reader);

		/*
		 * SHOW COOL BANNER
		 */
		//showBanner();


		/*
		 * STEP 4: LOAD ENVIRONMENT SETTINGS
		 */



		/*
		 * This next section sets all the settings available in the robot from the settings file
		 */



		 std::cout << "Config loaded from 'config.settings': version="
		              << reader.GetInteger("system", "version", -1) << ", debug level="
					  << reader.Get("logging", "debugLevel", "UNKNOWN") << ", name="
		              << reader.Get("user", "name", "UNKNOWN") << ", email="
		              << reader.Get("user", "email", "UNKNOWN") << ", pi=";

		              //<< reader.GetReal("user", "pi", -1) << ", active="
		              //<< reader.GetBoolean("user", "active", true) << "\n";



		//RUN BOOT METHODS:



		/*
		 * STEP LAST: STARTS SYSTEM OPERATIONS
		 */
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





 //================================================================================
 // Private Methods
 //================================================================================



 void setLogging(SettingsReader &reader){
	 print("Starting setLogging Method");
	//SET LOGGING VARIABLES
	blaze::DEBUG_ENABLED 	= reader.GetBoolean(LOGGING_SECTION, "DEBUG_ENABLED", true);
	blaze::DEBUG_LEVEL 		= reader.GetBoolean(LOGGING_SECTION, "DEBUG_LEVEL", "DEBUG");
	blaze::LOGFILE_ENABLED 	= reader.GetBoolean(LOGGING_SECTION, "LOGFILE_ENABLED", true);
	blaze::LOGFILE_NAME 	= reader.GetBoolean(LOGGING_SECTION, "LOGFILE_NAME", "./blaze_debug.log");
	blaze::APPEND_TO_LOG 	= reader.GetBoolean(LOGGING_SECTION, "APPEND_TO_LOG", false);

	/*
	 * SET LOGGING LEVELS by settings file...
	 */
	string logLevel 		= blaze::DEBUG_LEVEL;
	const char * strLogFile = blaze::LOGFILE_NAME.c_str();

	//Load from settings file or default to DEBUG1
	if(logLevel.length() >= 0){
		FILELog::ReportingLevel() = FILELog::FromString(logLevel);
	}

	//write to log
	if(blaze::LOGFILE_ENABLED){
		LOG_TO_FILE(log_file, strLogFile);
	}
	print("Debug Configuration Completed");
	debug(blaze::DEBUG_LEVEL);
 }


/*
 * bootSystemOperations
 * Function spins off a thread to start the system operations object...
 */
void bootSystemOperations(){
	debug("started bootloader::bootSystemOperations");
	//STEP 1: Define Handle to the Thread:
	pthread_t controllerEntry;
	//OPTIONAL: Define the data received back from pthread.
	void* result;
	//STEP 2: Create thread, pass reference, addr of the function and data
	if(pthread_create(&controllerEntry, NULL, &controller::entry, NULL)){
		cout <<"Failed to create the thread" << endl;
		return;
	}
	//STEP 3: Allow the Thread to complete...
	pthread_join (controllerEntry, &result);

	return;
}



//================================================================================
// HELPER METHODS
//================================================================================
/*
 * Generic Method for printing text to the console
 */
 void print (string msg){
	 cout << msg << endl;
 }
 void debug (string msg){
	 FILE_LOG(logDEBUG) << msg;
 }
 void info (string msg){
	 FILE_LOG(logINFO) << msg;
 }

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










/*
 * BLAZE BOT BANNER TEXT
 */
 void showBanner(){
	 info("                     _                    _");
	 info("                  ,/                        \\,");
	 info("        _________{(                          })_________");
	 info("       /.-------./\\                        //\\.-------.\\");
	 info("      //@@@@@@@//@@\\\\  )                (  //@@\\\\@@@@@@@\\\\");
	 info("     //@@@@@@@//@@@@>>/                  \\<<@@@@\\\\@@@@@@@\\\\");
	 info("    //O@O@O@O//@O@O//                      \\\\O@O@\\\\O@O@O@O\\\\");
	 info("  //OOOOOOOO//OOOO||          \\  /          ||OOOO\\\\OOOOOOOO\\\\");
	 info(" //O%O%O%O%//O%O%O%\\\\         ))((         //%O%O%O\\\\%O%O%O%O\\\\");
	 info("||%%%%%%%%//'  `%%%%\\\\       //  \\       //%%%%'   `\\\\%%%%%%%||");
	 info("((%%%%%%%((      %%%%%\\\\    ((    ))    //%%%%%       ))%%%%%%))");
	 info(" \\\\:::' `::\\      `:::::\\\\   \\)~~(/    //:::::'      //::' `:::/");
	 info("  )'     `;)'      (`  ` \\\\ `<@  @>' / / '  ')      `(;'     `(");
	 info("          (               \\`\\ )^^( /  /               )");
	 info("        _                  ) \\\\oo/   (");
	 info("       (@)                  \\  `'   /                      _");
	 info("       |-|\\__________________\\__^__<________oOo__________ (@)");
	 info("       |-|                                  VVV          \\|-|");
	 info("       |-|             2016 IEEE SOUTHEASTCON             |-|");
	 info("       |-|           UAB ELECTRICAL ENGINEERING           |-|");
	 info("       |-|                                                |-|");
	 info("       |-|      ¸.´¯`·.¸¸.·´¯`·.BLAZE.·´¯`·.¸¸.´¯`·.¸     |-|");
	 info("       |-|                                                |-|");
	 info("       |-|\\_____________________________________________  |-|");
	 info("       (@)                 / ,/ \\_____/ \\ ~\\/~         `\\|-|");
	 info("        ~             ___//^~      \\____/\\               (@)");
	 info("                     <<<  \\     __  <____/||               ~");
	 info("                               <   \\ <___/||");
	 info("                                  || <___//");
	 info("                                  \\ \\/__//");
	 info("                                   ~----~");
	 info("");
	 info("");
	 info("Starting Birmingham's Logistics Actuating Zone Evaluator ...");
	 	 return;
 }
