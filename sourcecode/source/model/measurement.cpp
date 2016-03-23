/*
 * measurement.cpp
 *
 *  Created on: Mar 20, 2016
 *      Author: Peter
 */


#include "../../include/globals.h"
#include "../../include/model/measurement.h"

namespace blaze {

measurement::~measurement() {
	// TODO Auto-generated destructor stub
}


void measurement::addDistance(int angle, double dist){

	this->distances[angle] = dist;
/*
	if(dist>1){
		double x1 = (dist*cos(angle)) + (-39);
		double y1 = (dist*sin(angle)) + (41);
		this->distances[angle] = sqrt( (pow(x1, 2) + pow(y1,2)) ); // dist;
		x[angle] = x1;
		y[angle] = y1;
	}else{
		this->distances[angle] = 0;
		x[angle] = 0;
		y[angle] = 0;
	}

*/
}

/*
 * Used to calculate the Minimum distance in position object
 */
double measurement::getMinimumInRange(int maximumAngle, int minimumAngle){
	int index;
	double minimum = 5000.0;

	for (int k=maximumAngle; k>=minimumAngle; k--){
			if (k<=0){
				index = 360+k;
			} else{
				index = k;
			}
			if ((this->distances[index]<minimum) && (this->distances[index] != 0)){
				minimum = this->distances[index];
			}

	}
	return minimum;
}
int measurement::getIndexOfMinimumInRange(int maximumAngle, int minimumAngle){
	int index;
	double minimum = 5000.0;
	int minIndex = 0;

	for (int k=maximumAngle; k>=minimumAngle; k--){
			if (k<=0){
				index = 360+k;
			} else{
				index = k;
			}
			if ((distances[index]<minimum) && (distances[index] != 0)){
				minimum = distances[index];
				minIndex = index;
			}

	}
	return minIndex;
}

/*
 * Used to calculate the Maximum distance in position object
 */
double measurement::getMaximumInRange(int maximumAngle, int minimumAngle){
	int index;
	double maximum = 0;
	int maxIndex = 0;

	for (int k=maximumAngle; k>=minimumAngle; k--){
			if (k<=0){
				index = 360+k;
			} else{
				index = k;
			}
				if (distances[index]>maximum){
					maximum = distances[index];
					maxIndex = index;
				}
	}

	return maximum;
}
int measurement::getIndexOfMaximumInRange(int maximumAngle, int minimumAngle){
	int index;
	double maximum = 0;
	int maxIndex = 0;

	for (int k=maximumAngle; k>=minimumAngle; k--){
			if (k<=0){
				index = 360+k;
			} else{
				index = k;
			}
				if (distances[index]>maximum){
					maximum = distances[index];
					maxIndex = index;
				}
	}

	return maxIndex;
}
/*
 * Used to calculate the Average distance in position object
 */
double measurement::calculateAverageInRange(int maximumAngle, int minimumAngle){
	int index;
	double sum = 0;
	double count;


	for (int k=maximumAngle; k>=minimumAngle; k--){
			if (k<=0){
				index = 360+k;
			} else{
				index = k;
			}
			sum += distances[index];
			if(distances[index] != 0){
				count++;
			}

	}

	return sum/count;
}

/*
 * Used to calculate the Theta angle, compared to a wall.  The Cardinal is the the
 */
double measurement::calculateThetaInRange(int maximumAngle, int minimumAngle, Face f){

	double theta = -1;
	int index;
	index = this->getIndexOfMinimumInRange(maximumAngle, minimumAngle);

	if(f == Face::Front){
		theta = 90-index;
	}
	if(f == Face::Rear){
		theta = 270 - index;
	}
	if(f == Face::Left){
		theta = 180-index;
	}
	if(f == Face::Right){
		theta = 360-index;
	}

	if(theta < 0){
		theta = theta + 360;
	}

	return theta;
}


}
