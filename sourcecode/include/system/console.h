/*
 * console.h
 *
 *  Created on: Oct 7, 2015
 *      Author: Peter
 */

#ifndef INCLUDE_SYSTEM_CONSOLE_H_
#define INCLUDE_SYSTEM_CONSOLE_H_

#include <iostream>
#include <string>


#ifdef LOG1
#include "system/log1.h"
#else
#include "../../include/system/log.h"
#endif

using namespace std;


class console{
public:
	static void debug(std::string);
	static void info(std::string);
	static void warn(std::string);
	static void error(std::string);
	static void print(std::string);
	static std::string currentDateTime();


};




#endif /* INCLUDE_SYSTEM_CONSOLE_H_ */
