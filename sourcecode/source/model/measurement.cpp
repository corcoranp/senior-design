/*
 * measurement.cpp
 *
 *  Created on: Mar 20, 2016
 *      Author: Peter
 */

#include "../../include/globals.h"
#include "measurement.h"

measurement::measurement() {
	// TODO Auto-generated constructor stub

}

measurement::~measurement() {
	// TODO Auto-generated destructor stub
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
			if ((distances[index]<minimum) && (distances[index] != 0)){
				minimum = distances[index];
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
 * Used to calculate the Theta angle, compared to a wall
 */
double measurement::calculateThetaInRange(int maximumAngle, int minimumAngle){

	int index;
	index = this->getIndexOfMinimumInRange(maximumAngle, minimumAngle);




	//find the quadrant the min index is in...

	// Check to see if maximum angle is LEFT maximum
	if(	maximumAngle==LEFT_MAX){
		resultantValue = minIndex - 180;
	}
	//check to see if maximum is the RIGHT maximum
	else if(maximumAngle == RIGHT_MAX) {
		if(minIndex<=0){
			resultantValue = minIndex;
		}else{
			resultantValue = minIndex - 360;
		}
	}
	else if(maximumAngle==FRONT_MAX){
		resultantValue = minIndex - 90;
	}
	else{
		resultantValue = minIndex - 270;
	}
	//if(angle_in_radians != 0){
	//	resultantValue *= 0.0174533;
	//}

	//return
	return resultantValue;
}
