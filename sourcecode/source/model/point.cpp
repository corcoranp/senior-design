/*
 * Point
 * Copyright: © 2015
 * http://sandbox.mc.edu/~bennet/cs220/codeex/index.html
 *
 * Jul 2, 2015 - Initial Coding
 *
 */

#include <iostream>
#include <math.h>
#include "model/point.h"


// Constructor uses default arguments to allow calling with zero, one,
// or two values.

double Point::x() { return xval; }
double Point::y() { return yval; }

	// Distance to another point.  Pythagorean thm.
double Point::dist(Point other) {
			double xd = xval - other.xval;
			double yd = yval - other.yval;
			return sqrt(xd*xd + yd*yd);
}

// Add or subtract two points.
Point Point::add(Point b)
{
	return Point(xval + b.xval, yval + b.yval);
}
Point Point::sub(Point b)
{
	return Point(xval - b.xval, yval - b.yval);
}

void Point::move(double a, double b)
{
	xval += a;
	yval += b;
}


