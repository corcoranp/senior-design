/*
 * LidarIO.h
 *
 *  Created on: Feb 7, 2016
 *      Author: Peter
 */

#ifndef LIDARIO_H_
#define LIDARIO_H_

#include <stdio.h>
#include <string>
#include <fcntl.h>
#include <termios.h>
#include <vector>
#include "../globals.h"
using namespace std;


extern int lidar_fd;
int lidar_open(int deviceIndex);
void lidar_close();



namespace blaze {

class  lidarIO {
public:

	lidarIO(){
		isConnected = false;
		zeroRef = 0;
		port = "";
	}
	lidarIO(char * p){
		isConnected = false;
		zeroRef = 0;
		port = p;
	}
	lidarIO(string p){
		isConnected = false;
		zeroRef = 0;
		port = string2char(p);
	}

	static int lidarFileDescriptor;
	void readData();
	double * getData(int fd, double *returnArray);
	int connect();
	void disable();

	int set_interface_attribs(int fd);
	bool isConnected;
	char * port;
	int zeroRef;

private:
	void debug(char * msg);

};



}
#endif /* LIDARIO_H_ */
