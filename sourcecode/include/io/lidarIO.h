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
#include "../system/workQueue.h"
#include "../model/measurement.h"
#include "pthread.h"
using namespace std;


extern int lidar_fd;
int lidar_open(int deviceIndex);
void lidar_close();



namespace blaze {

class  lidarIO {

public:

	//lidarIO( workqueue<measurement*>& queue) : m_queue(queue)  {
	lidarIO( )  {
		isConnected = false;
		port = "";
		isPaused = false;
	}
	lidarIO(char * p) {
		isConnected = false;
		port = p;
		isPaused = false;
	}
	lidarIO(string p)  {
		isConnected = false;
		port = string2char(p);
		isPaused = false;
	}
	virtual ~lidarIO();

	/**
	* Data Functions
	*/
	static int lidarFileDescriptor;
	double * getData(int fd, double *returnArray);

	//static const int zeroRef = ZERO_REF;


	/**
	 * Connection functions/properties
	 */
	int connect();
	void disconnect();

	void disable();
	int set_interface_attribs(int fd);

	void showDistances(bool enable);
	void enableMotor (bool enable);
	void showRaw (bool enable);
	void pause(bool enable);


	bool isConnected;
	bool isPaused;
	char * port;


private:
	void debug(char * msg);


};



}
#endif /* LIDARIO_H_ */
