/*
 * Senior Design Project Bootloader
 * By: Peter Corcoran
 * Created: June 24, 2015
 *
 * Initial c++ coding for 2015-2016 UAB EE Senior Design
 *
 */
#include <iostream>

#include <pthread.h>		//Needed for threading
#include <stdio.h>			//Needed for threading
#include <unistd.h>			//Needed for usleep

/*
 * BLAZEBOT INCLUDES
 */
#include "../include/robotcore.h"
#include "../include/qrcode.h"
#include "../include/zone.h"
#include "../include/sysops.h"
#ifdef LOG1
#include "log1.h"
#else
#include "log.h"
#endif

//used for command options
#include <algorithm>

using namespace std;

/*
 * DECLARATIONS - MY FUNCTIONS
 */
void bootSystemOperations();

//Commandline options
char* getCmdOption(char ** begin, char ** end, const std::string & option);
bool cmdOptionExists(char** begin, char** end, const std::string& option);

//DEBUG OPTIONS---------
void print(string);
void debug(string);
void info(string);
FILE * log_file;

void showBanner();

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
//	 cout << "\x1B[2J\x1B[H";					//clear screen
	char * item;
	//std::string Str = std::string(cStr);
	try
	{

		char * logLevel = getCmdOption(argv, argv + argc, "-logLevel");
		if(logLevel){
			FILELog::ReportingLevel() = FILELog::FromString(logLevel ? logLevel : "DEBUG1");
		}
		//write to log
		if(cmdOptionExists(argv, argv+argc, "-log")){
			LOG_TO_FILE(log_file,"./blazebot.log");
		}


		showBanner();
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




void bootSystemOperations(){
debug("started bootloader::bootSystemOperations");
	 //STEP 1: Define Handle to the Thread:
	 pthread_t sysopsEntry;
	 //OPTIONAL: Define the data received back from pthread.
	 void* result;
	 //STEP 2: Create thread, pass reference, addr of the function and data
	 if(pthread_create(&sysopsEntry, NULL, &sysops::entry, NULL)){
		 cout <<"Failed to create the thread" << endl;
		 return;
	 }
	 //STEP 3: Allow the Thread to complete...
	 pthread_join (sysopsEntry, &result);

	 return;
}




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

 char* getCmdOption(char ** begin, char ** end, const std::string & option)
 {
     char ** itr = std::find(begin, end, option);
     if (itr != end && ++itr != end)
     {
         return *itr;
     }
     return 0;
 }

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
