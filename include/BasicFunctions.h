/* 
 * File:   BasicFunctions.h
 * Author: Pieter Loubser
 *
 * Created on December 30, 2015, 5:48 PM
 */

#ifndef BASICFUNCTIONS_H
#define	BASICFUNCTIONS_H

//Libraries
#include <cstdlib>
#include <errno.h>
#include <string.h>
#include <string>
#include <sstream>
#include <iostream>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <cmath>
#include <chrono>
#include <thread>

using namespace std;

#ifndef ENABLE_DEBUG_OUTPUT 
// Set to 1 to print some debug messages, or 0 to disable them.
#define ENABLE_DEBUG_OUTPUT 0
#endif

#ifndef ROUND_2_INT
#define ROUND_2_INT(f) ((int)(f >= 0.0 ? (f + 0.5) : (f - 0.5)))
#endif

#ifndef ANG_TO_RAD
#define ANG_TO_RAD(angle) ((angle*M_PI)/180.0)
#endif

#ifndef RAD_TO_ANG
#define RAD_TO_ANG(x) ((x*180.0)/M_PI)
#endif

#ifndef NORM_ATAN2
#define NORM_ATAN2(x) ((x < 0.0)?(x+(2*M_PI)):x)
#endif

#ifndef uint8_t
typedef unsigned char uint8_t;
#endif

#ifndef uint16_t
typedef unsigned short int uint16_t;
#endif

#ifndef uint
typedef unsigned int uint;
#endif

#ifndef ulong
typedef unsigned long int ulong;
#endif

#ifndef byte
typedef unsigned char byte;
#endif

#ifndef Point2D

typedef struct Point2D_t {
    int x;
    int y;
} Point2D;
#endif

#ifndef Point3D
typedef struct Point3D_t {
    int x;
    int y;
    int z;
} Point3D;
#endif

#ifndef Location

typedef struct location_t {
    Point2D point;
    double angle;
} Location;
#endif

namespace gpio {
	enum DIRECTION{ INPUT, OUTPUT};
	enum VALUE{LOW=0, HIGH=1};
};

extern int piBoardVersion(void);
extern void pinModeGPIO(int pin, int dir);
extern int GPIOExport(int pin);
extern int GPIOUnexport(int pin);
extern int GPIODirection(int pin, int dir);
extern int GPIORead(int pin);
extern int GPIOWrite(int pin, int value);

using namespace std;

class Timer {
private:
    timeval startTime;
    bool started;

public:
    
    /**
     * Timer constructor;
     */
    Timer(){
        started = false;
    }

    /**
     * Start the timer functions.
     */
    void start(){
        gettimeofday(&startTime, NULL);
        started = true;
    }

    /**
     * Get the time difference between the start function call and this function. 
     * @return Time is micro seconds from start function call.
     */
    long getTime(){
        if(!started){
            return 0L;
        }
        timeval endTime;

        gettimeofday(&endTime, NULL);

        return endTime.tv_usec - startTime.tv_usec;
    }
};


#endif	/* BASICFUNCTIONS_H */

