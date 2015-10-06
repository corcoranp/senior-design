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
	double xval, yval, zval, theta;
public:
	// Constructor uses default arguments to allow calling with zero, one,
	// or two values.
	Point(double x = 0.0, double y = 0.0) {
			xval = x;
			yval = y;
	}

	double x();
	double y();

	// Distance to another point.  Pythagorean thm.
	double dist(Point other);
	Point add(Point b);
	Point sub(Point b);
	void move(double a, double b);
};


#endif /* INCLUDE_POINT_H_ */
