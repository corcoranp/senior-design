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
#include "../model/waypoint.h"

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


	static void *scan(void *value);
	static void *localize(void *value);
	static workqueue<measurement*>* m_queue;


	/**
	 * Data Items
	 */
	bool isLocalizing;

	bool hasQueue;
	int map_xmax, map_ymax;
	waypoint* lastKnownPoint;
	Face referenceFace;


	double data[370];
	measurement *last_measure;
	PortConfig portCfg;

	void startLocalizing();



	/**
	 * Data functions
	 */
	void offset_correction();
	double getPosition (Face f, DistType dt);
	void updateLocation();



	/**
	 * Moving Functions
	 */
	void stopNow();
	void move(WALL_FOLLOWING following_mode, int distance, int angle, int forward_target_distance  );
	void moveUntil(int forward_target_distance, MOVEMENT dir, SPEED speed  );
	void alignToFace(Face f);
	void turn(int targetAngle, double radius);

	void moveTo(waypoint p);

	/**
	 * Complex moves
	 */
	PortConfig getCurrentPort();

	PortConfig determinePort();
	PortConfig determinePort(measurement *m);
	void navigateThroughTunnel();
	void small_turn();

private:
	bool hasPortBeenFound = false;

};

} /* namespace blaze */

#endif /* NAVIGATIONCONTROLLER_H_ */
