/*
 * Waypoint.cpp
 *
 *  Created on: Oct 6, 2015
 *      Author: Peter
 *
 *	A waypoint is a position & direction on the port (x,y,theta)
 */

#include <string>
#include "../../include/model/waypoint.h"

using namespace blaze;
using namespace std;

double waypoint::getX() { return xval; }
double waypoint::getY() { return yval; }
double waypoint::getTheta() { return theta; }

void waypoint::setX(double val){
	xval = val;
}
void waypoint::setY(double val){
	yval = val;
}
void waypoint::setTheta(double val){
	theta = val;
}

string waypoint::toString(){
	return "(" ;//+ to_string(xval) + "," + to_string(yval) + "," + to_string(theta) + ")";
}
