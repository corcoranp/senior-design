/*
 * lidar_io.cpp
 *
 *  Created on: Feb 7, 2016
 *      Author: Peter
 */


#include "../../include/io/lidarIO.h"
#include "../include/system/console.h"
#include "../../include/model/pdata.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <vector>
#include <fstream>
#include <unistd.h>

using namespace std;
using namespace blaze;

int lidar_fd = -1;
pdata *pdata::s_instance = 0;


lidarIO::~lidarIO() {
	close(this->lidarFileDescriptor);
}

/* Usage:

 	#include "./lidar_io.h"
 	...
	lidarIO lidar("/dev/tty.usbmodem12341");
	lidar.connect();
	lidar.getData(lidar.lidarFileDescriptor);
 */
int lidarIO::lidarFileDescriptor = -1;





double * lidarIO::getData(int fd, double *returnArray){

	/*
	 * Control flag for determining when to start the "counter"
	 */
	bool startCount = false;
	/*
	 * Control flag to keep loop going until end is reached
	 */
	bool dontStopLoop = true;
	int items = 0;

	/*
	 * Main while loop for getting data from lidar
	 * Should not stop until 360 degrees collected...
	 */
	while (dontStopLoop) {
		//sleep thread for a moment...
		usleep(500); //sleep thread

		//character buffer
		char buffer[64] = {0};
		// integer representing where in the buffer we are...
		int pos = 0;

		/*
		 * Goal Variables
		 * angle is the angle being read
		 * dist is the distance value of that angle
		 */
		string angle = "";
		string dist;


		string delimiter = " "; //separate data by spaces
		size_t posi = 0;
		string token; 			//the "token" is the current data item
		int count;
		bool isFinished;
		isFinished = false;

		//console::debug("LidarIO: loop through the buffer");

		/*
		 * Loop through the buffer and find the line terminator
		 *
		 */
		//while( pos < 63 ) {
		while( pos <= 64 ) {
			read(fd, buffer+pos, 1); //read one more character
			// Note you should be checking the result
			if( buffer[pos] == '\n' || buffer[pos] == '\r') break;
				pos++; //update the position
		}
		string s(buffer); 	// 	convert buffer into string
		count = 1;			//	start count at 1

		//Parse the Lidar Data...separate by space...
		while ((posi = s.find(delimiter)) != std::string::npos) {

			token = s.substr(0, posi);
			//console::debug("Parsing token data " + token.length());
			if(token.length() < 1) break;

			//check for "Time" string
			if(token == "Time"){
				//console::debug("time: ");
				isFinished = true;
			}
			// First delimited item:
			if(count == 1 && !isFinished){
				token.pop_back();
				angle = token;

				items++;
			}
			// Second delimited item:
			if(count == 2 && !isFinished){
				dist = token;
			}
			//Erase previous token
			s.erase(0, posi + delimiter.length());
			count++;
		} //end while

		if(angle.length() > 0){
			//write out data if needed...

			//console::debug("adjusted angle: " + to_string(items) + " dist: " + dist  );
			//string str = angle;
		}
		if(!isFinished && angle != "" && startCount){
			double d = atof (dist.c_str()); //convert string to double
			if(items < 360){
				returnArray[items] = d;
			//	pdata::instance()->updateElement(items, d);
			}
		}

		if(startCount && angle == to_string(zeroRef)){
			console::debug("lidarIO: Reached end of lidar data");
			dontStopLoop = false;  //stop the main while loop...

		} //end of if


		//Start counting angles...
		if(angle == to_string(zeroRef)){
			//Lidar alignment...
			//console::debug("lidarIO:  angle is at zeroRef");
			startCount = true; //loop until first end...
			items = 0;
		}

		count = 0; 	//reset angle data...
		angle ="";
		dist= "";
	}
	console::debug("lidarIO: finished reading lidar data");
}



int lidarIO::connect(){
	console::debug( lidarIO::port);
	console::debug("LidarIO - open file descriptor");

	lidarFileDescriptor = open( lidarIO::port, O_RDWR | O_NOCTTY | O_NONBLOCK ) ;
	fcntl(lidarFileDescriptor, F_SETFL, FNDELAY);

	console::debug("LidarIO: set interface attribs");
	set_interface_attribs(lidarFileDescriptor);

	//send lidar control commands...
	this->isConnected = true;
	this->showRaw(false);
	this->showDistances(true);
	return lidarFileDescriptor;
}

void lidarIO::showDistances(bool enable){

	char showDist[] = "ShowDist \n\r";
	char hideDist[] = "HideDist \n\r";
	if(!this->isConnected) return;

	if(enable){
		console::debug("LIDAR: show Distance");
		write(lidarFileDescriptor, showDist, sizeof(showDist));
		console::debug("LIDAR: show Distance Finished");
	}else{
		console::debug("LIDAR: hide Distance");
		write(lidarFileDescriptor, hideDist, sizeof(hideDist));
	}
}
void lidarIO::enableMotor (bool enable){
	console::debug("LIDAR: enableMotor");
	char motorOn[] = "MotorOn \n\r";
	char motorOff[] = "MotorOff \n\r";

	if(!this->isConnected) return;

	if(enable){
		write(lidarFileDescriptor, motorOn, sizeof(motorOn));
	}else{
		write(lidarFileDescriptor, motorOff, sizeof(motorOff));
	}
}
void lidarIO::showRaw (bool enable){

	char showRaw[] = "ShowRaw \n\r";
	char hideRaw[] = "HideRaw \n\r";

	if(!this->isConnected) return;

	if(enable){
		console::debug("LIDAR: show Raw");
		write(lidarFileDescriptor, showRaw, sizeof(showRaw));
	}else{
		console::debug("LIDAR: hide Raw");
		write(lidarFileDescriptor, hideRaw, sizeof(hideRaw));
	}
}

void lidarIO::pause(bool enable){
	showDistances(!enable);
	showRaw(!enable);
	this->isPaused = enable;
}




void lidarIO::disconnect(){
	//not really disconnecting...
	console::debug("closing file descriptor ");
	try{
		close(this->lidarFileDescriptor);
		this->isConnected = false;
	}catch ( const std::exception& e ){
		console::error(e.what());
	}
}

/**
 * Function to disable the LIDAR
 */
void lidarIO::disable(){
	console::debug("Disable Lidar");

	lidarFileDescriptor = open( lidarIO::port, O_RDWR | O_NOCTTY | O_NONBLOCK ) ;
	fcntl(lidarFileDescriptor, F_SETFL, FNDELAY);

	console::debug("LidarIO: set interface attribs");
	set_interface_attribs(lidarFileDescriptor);

	char stopMotor[] = "MotorOff \n\r";

	write(lidarFileDescriptor, stopMotor, sizeof(stopMotor));
	console::debug("LIDAR: Disabled");

}

/**
 * Function to set Serial Communication attributes
 */
int lidarIO::set_interface_attribs (int fd)
{
        struct termios tty;
        struct termios tty_old;

        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
        	isConnected = false;
        	cout << "Error " << errno << " from tcgetattr: " << strerror(errno) << endl;
            return -1;
        }

    	/* Save old tty parameters */
    	tty_old = tty;

    	/* Set Baud Rate */
    	cfsetospeed (&tty, (speed_t)B115200);
    	cfsetispeed (&tty, (speed_t)B115200);

    	/* Setting other Port Stuff */
		tty.c_cflag     &=  ~PARENB;            // Make 8n1
		tty.c_cflag     &=  ~CSTOPB;
		tty.c_cflag     &=  ~CSIZE;
		tty.c_cflag     |=  CS8;

		tty.c_cflag     &=  ~CRTSCTS;           // no flow control
		tty.c_cc[VMIN]   =  1;                  // read doesn't block
		tty.c_cc[VTIME]  =  5;                  // 0.5 seconds read timeout
		tty.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines

		/* Make raw */
		cfmakeraw(&tty);

		/* Flush Port, then applies attributes */
		tcflush( fd, TCIFLUSH );
		if ( tcsetattr ( fd, TCSANOW, &tty ) != 0) {
			isConnected = false;
			cout << "Error " << errno << " from tcsetattr";
		}
		if (fd == -1) {
			debug("Unable to open port. \n");
			isConnected = false;
		}
		else {
			debug("Port opened.\n") ;
			isConnected = true;
		}

        return 0;
}








/*
 * Send Command
 */
/*
	Help - Show the list of commands available
	ShowConfig - Show the running configuration
	SaveConfig - Save the running configuration to EEPROM
	ResetConfig - Restore the original configuration
	SetRPM - Set the desired rotation speed (min: 200, max: 300)
	SetKp - Set the proportional gain
	SetKi - Set the integral gain
	SetKd - Set the derivative gain
	SetSampleTime - Set the frequency the PID is calculated (ms)
	ShowRPM - Show the rotation speed
	HideRPM - Hide the rotation speed
	ShowDist - Show the distance data
	HideDist - Hide the distance data
	ShowAngle - Show distance data for a specific angle (0 - 359 or 360 for all)
	MotorOff - Stop spinning the lidar
	MotorOn - Enable spinning of the lidar
	HideRaw - Stop outputting the raw data from the lidar
	ShowRaw - Enable the output of the raw lidar data
 */

/*
 * PRIVATE FUNCTIONS
 */



void lidarIO::debug(char * msg){

	cout << msg << endl;
}

