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
#include "../../include/model/point.h"



// Constructor uses default arguments to allow calling with zero, one,
// or two values.

double Point::x() { return xval; }
double Point::y() { return yval; }
double Point::z() { return zval; }
double Point::theta() { return thetaval; }

	// Distance to another point.  Pythagorean thm.
double Point::dist(Point other) {
			double xd = xval - other.xval;
			double yd = yval - other.yval;
			return sqrt(xd*xd + yd*yd);
}

// Add or subtract two points.
Point Point::add(Point b)
{
	return Point(xval + b.xval, yval + b.yval, zval + b.zval, thetaval + b.thetaval);
}
Point Point::sub(Point b)
{
	return Point(xval - b.xval, yval - b.yval, zval - b.zval, thetaval - b.thetaval);
}

void Point::move(double a, double b)
{
	xval += a;
	yval += b;
}

void Point::move(double a, double b, double c, double d){
	xval +=a;
	yval +=b;
	zval +=c;
	thetaval +=d;
}


