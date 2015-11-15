/*
 * filesys.h
 *
 *  Created on: Jul 23, 2015
 *      Author: Peter
 */

#ifndef INCLUDE_IO_FILESYS_H_
#define INCLUDE_IO_FILESYS_H_

#ifdef LOG1
#include "system/log1.h"
#else
#include "../../include/system/log.h"
#endif

#include <string>

using namespace std;

namespace blaze {
	void console (string msg);
	string read(string basepath, string filename);
	string read(string fullpath);

	int write(string basepath, string filename, string inputVal);
	int write(string basepath, string filename, int inputVal);
	int write(string fullPath, string inputVal);
	int write(string fullPath, int inputVal);

}


#endif /* INCLUDE_IO_FILESYS_H_ */

