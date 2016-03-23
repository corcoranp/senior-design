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
#include "MotorController.h"

namespace blaze {

class NavigationController {
private:
	lidarIO *lidar;


public:

	NavigationController(MotorController *mc);
	virtual ~NavigationController();
	void init();

	static MotorController *motorController;
	void addMotorController(MotorController *mc);


	angleRange currentRightPos;
	angleRange currentFrontPos;
	angleRange currentLeftPos;
	angleRange currentBackPos;

	static void *localize(void *value);
	static workqueue<measurement*>* m_queue;
	//void enableLocalication(bool enable);
	//void addQueue(workqueue<measurement*>* q);

	bool isLocalizing;
	bool hasQueue;

	void stopNow();

	/**
	 * Data functions
	 */
	double data[370];
	void offset_correction();
	double getPosition (Face f, DistType dt);
	void updateLocation();


	void calculateAverage(angleRange *p);
	void calculateMinimum(angleRange *p);
	void calculateMaximum(angleRange *p);
	void calculateTheta(angleRange *p);

	/**
	 * Moving Functions
	 */
	void move(WALL_FOLLOWING following_mode, int distance, int angle, int forward_target_distance  );
	void moveUntil(int forward_target_distance, MOVEMENT dir  );
	void alignToFace(Face f);
	void turn(int targetAngle, double radius);

	/**
	 * Complex moves
	 */
	PortConfig determinePort();
	void navigateThroughTunnel();
	void small_turn();


};

} /* namespace blaze */

#endif /* NAVIGATIONCONTROLLER_H_ */
