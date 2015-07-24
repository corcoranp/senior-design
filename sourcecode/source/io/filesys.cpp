/*
 * filesys.cpp
 *
 *  Created on: Jul 23, 2015
 *      Author: Peter
 *
 *	Methods are used to read/write to file system.
 *
 *	References;
 *	ifstream  http://www.cplusplus.com/reference/fstream/ifstream/
 *	http://www.cplusplus.com/reference/istream/istream/getline/
 *
 */

#include "io/filesys.h"
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

namespace blaze {

	/*
	 * READ FUNCTIONS
	 */
	string read(string basepath, string filename){
		console("started read method");

		return read((basepath + filename));
	}
	string read(string fullpath){
		console("started read method");

		ifstream fstrm;	//file stream
		string retval;	//return value

		fstrm.open(fullpath.c_str()); //open the file for reading

		if(!fstrm.is_open()){
			//ERROR
			perror("GPIO ERROR: FAILED TO OPEN FILE FOR READ");
		}

		getline(fstrm, retval); //read line
		fstrm.close();			//close stream

		return retval; //return value
	}

	/*
	 * WRITE FUNCTIONS
	 */
	int write(string basepath, string filename, string inputVal){
		console("started write method");
		return write((basepath + filename), inputVal);
	}

	int write(string basepath, string filename, int inputVal){
		console("started write method");
		return write((basepath + filename), inputVal);
	}

	int write(string fullPath, string inputVal){
		console("started write method");
		ofstream fstrm;

		fstrm.open(fullPath.c_str());
		if (!fstrm.is_open()){
		   perror("GPIO ERROR: FAILED TO OPEN FILE FOR WRITE ");
		   return -1;
		}
	   fstrm << inputVal;
	   fstrm.close();
	   return 0;
	}
	int write(string fullPath, int inputVal){
		console("started write method");
		stringstream s;
		s << inputVal;
		return write(fullPath, s.str());
	}



 void console (string msg){
	 FILE_LOG(logDEBUG) << msg;
 }
} /* namespace blaze */
