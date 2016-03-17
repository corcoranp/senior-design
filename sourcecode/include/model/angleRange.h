/*
 * position.h
 *
 *  Created on: Mar 17, 2016
 *      Author: Peter
 */

#ifndef POSITION_H_
#define POSITION_H_

class angleRange {
public:
	angleRange();
	virtual ~angleRange();

	int MaximumAngle;
	int MinimumAngle;

	double averageDistance;
	double minimumDistance;
	double maximumDistance;
	double angleOffset;
	double radianOffset;



};

#endif /* POSITION_H_ */
