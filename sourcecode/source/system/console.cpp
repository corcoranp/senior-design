/*
 * console.cpp
 *
 *  Created on: Oct 7, 2015
 *      Author: Peter
 */
#include <iostream>
#include <string>
#include <stdio.h>
#include <time.h>

#include "system/console.h"
#include "blazecore.h"

using namespace blaze;

/*
 * Generic Method for printing text to the console
 */
 void console::print (string msg){
	 cout << msg << endl;
 }
 void console::debug (string msg){
	 cout << "DEBUG: " << blaze::LOGFILE_ENABLED << endl;
	if(blaze::LOGFILE_ENABLED){
		FILE_LOG(logDEBUG) << msg;
	}
	if(blaze::CONSOLE_ENABLED){
		print(currentDateTime() + " - " + msg);
	}
 }
 void console::info (string msg){
	if(blaze::LOGFILE_ENABLED){
		FILE_LOG(logINFO) << msg;
	}
	if(blaze::CONSOLE_ENABLED){
		print(msg);
	}
 }

 void console::warn (string msg){
	if(blaze::LOGFILE_ENABLED){
		FILE_LOG(logWARNING) << msg;
	}
	if(blaze::CONSOLE_ENABLED){
		print(msg);
	}
 }

 void console::error (string msg){
	if(blaze::LOGFILE_ENABLED){
		FILE_LOG(logERROR) << msg;
	}
	if(blaze::CONSOLE_ENABLED){
		print(msg);
	}
 }

std::string console::currentDateTime() {
     time_t     now = time(0);
     struct tm  tstruct;
     char       buf[80];
     tstruct = *localtime(&now);
     // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
     // for more information about date/time format
     strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

     return buf;
 }

