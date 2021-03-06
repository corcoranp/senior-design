/*
 * Point.h
 *
 *  Created on: Jul 1, 2015
 *      Author: Peter
 */

#ifndef INCLUDE_POINT_H_
#define INCLUDE_POINT_H_


class Point {

private:
	double xval, yval, zval, thetaval;
public:
	// Constructor uses default arguments to allow calling with zero, one,
	// or two values.
	Point(double x = 0.0, double y = 0.0, double z = 0.0, double theta = 0.0) {
			xval = x;
			yval = y;
			zval = z;
			thetaval = theta;
	}

	double x();
	double y();
	double z();
	double theta();

	// Distance to another point.  Pythagorean thm.
	double dist(Point other);
	Point add(Point b);
	Point sub(Point b);
	void move(double a, double b);
	void move(double a, double b, double c, double d);
};


#endif /* INCLUDE_POINT_H_ */
