/*
 * measurement.h
 *
 *  Created on: Mar 20, 2016
 *      Author: Peter
 */

#ifndef MEASUREMENT_H_
#define MEASUREMENT_H_

#include <vector>
#include <cstdlib>
#include "../enums.h"
#include "../model/point.h"
#include <time.h>


using namespace std;
namespace blaze{

/**
 * A Measurement is a single measure of lidar data, and the methods used
 * to calculate location
 */
class measurement {
public:
	measurement(){
		event_time = 0;

	};
	virtual ~measurement();

	//Lidar distance measures
	double distances[361];
	double odistances[361];
	double x[361];
	double y[361];
	time_t event_time;

	void addDistance(int angle, double dist);


	double 	getMinimumInRange(int maximumAngle, int minimumAngle);
	int 	getIndexOfMinimumInRange(int maximumAngle, int minimumAngle);
	double 	getMaximumInRange(int maximumAngle, int minimumAngle);
	int 	getIndexOfMaximumInRange(int maximumAngle, int minimumAngle);
	double 	calculateAverageInRange(int maximumAngle, int minimumAngle);
	double 	calculateThetaInRange(int maximumAngle, int minimumAngle, Face f);



private:

};
}
#endif /* MEASUREMENT_H_ */
