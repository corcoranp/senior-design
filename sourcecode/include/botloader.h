/*
 * SeniorDesign.h
 *
 *  Created on: Jun 26, 2015
 *      Author: Peter
 */
#ifndef INCLUDE_BOTLOADER_H_
#define INCLUDE_BOTLOADER_H_

#ifdef _WIN32 // note the underscore: without it, it's not msdn official!
    // Windows (x64 and x86)
	#include <windows.h>		//Needed for threading
#elif __unix__ // all unices, not all compilers
    // Unix
#elif __linux__
    // linux
	#include <pthread.h>		//Needed for threading

#elif __APPLE__
    // Mac OS, not sure if this is covered by __posix__ and/or __unix__ though...
#endif


#include <iostream>
#include <string>

#include <stdio.h>			//Needed for threading
#include <unistd.h>			//Needed for usleep
//used for command options
#include <algorithm>
#include <system/SettingsReader.h>

#ifdef LOG1
#include "system/log1.h"
#else
#include "system/log.h"
#endif

//================================================================================
// SYSTEM METHOD DECLARATION
//================================================================================

char* getCmdOption(char ** begin, char ** end, const std::string & option);		// Gets command line options
bool cmdOptionExists(char** begin, char** end, const std::string& option);		// Checks to see if command line option exists

void setLogging(SettingsReader &reader);
void bootSystemOperations(); 					//boot base system functions
void showBanner();								//show the application banner




//================================================================================
// Member Fields
//================================================================================

FILE * log_file;								// Log file reference



#endif /* INCLUDE_BOTLOADER_H_ */
