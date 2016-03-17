/*
 * NavigationController.h
 *
 *  Created on: Mar 12, 2016
 *      Author: Peter
 */

#ifndef NAVIGATIONCONTROLLER_H_
#define NAVIGATIONCONTROLLER_H_

#include "../enums.h"
#include "../io/lidarIO.h"
#include "../model/angleRange.h"
#include "RobotController.h"

namespace blaze {

class NavigationController {
private:

	lidarIO lidar;
	MotorController* motorController;

	void calculateAverage(angleRange *p);
	void calculateMinimum(angleRange *p);
	void calculateMaximum(angleRange *p);
	void calculateTheta(angleRange *p);

public:

	NavigationController();
	NavigationController(MotorController *mc);
	virtual ~NavigationController();
	void init();

	angleRange currentRightPos;
	angleRange currentFrontPos;
	angleRange currentLeftPos;
	angleRange currentBackPos;

	double data[370];

	PortConfig determinePort();
	double getPosition (Face f, DistType dt);
	void moveToFirstPosition();
	void move(WALL_FOLLOWING following_mode, int distance, int angle, int forward_target_distance  );
	void alignToFace(Face f);
};

} /* namespace blaze */

#endif /* NAVIGATIONCONTROLLER_H_ */
