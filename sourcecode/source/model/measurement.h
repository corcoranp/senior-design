/*
 * measurement.h
 *
 *  Created on: Mar 20, 2016
 *      Author: Peter
 */

#ifndef MEASUREMENT_H_
#define MEASUREMENT_H_

#include <time.h>

/**
 * A Measurement is a single measure of lidar data, and the methods used
 * to calculate location
 */
class measurement {
public:

	measurement();
	virtual ~measurement();

	//Lidar distance measures
	double distances[360];
	time_t event_time;


	double 	getMinimumInRange(int maximumAngle, int minimumAngle);
	int 	getIndexOfMinimumInRange(int maximumAngle, int minimumAngle);
	double 	getMaximumInRange(int maximumAngle, int minimumAngle);
	int 	getIndexOfMaximumInRange(int maximumAngle, int minimumAngle);
	double 	calculateAverageInRange(int maximumAngle, int minimumAngle);
	double 	calculateThetaInRange(int maximumAngle, int minimumAngle);



private:

};

#endif /* MEASUREMENT_H_ */
