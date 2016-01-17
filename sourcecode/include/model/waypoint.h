/*
 * waypoint.h
 *
 *  Created on: Oct 6, 2015
 *      Author: Peter
 */

#ifndef INCLUDE_MODEL_WAYPOINT_H_
#define INCLUDE_MODEL_WAYPOINT_H_

#include <vector>
#include <sstream>
#include <stdio.h>

using namespace std;
namespace blaze {


class waypoint {
	private:
		double xval, yval, theta;

	public:
		//waypoint constructor that accepts a comma delimited string
		waypoint(string commaDelimited){
			std::istringstream ss(commaDelimited);
			std::vector<double> vect;
			std::string token;
			double di;

			while (ss >> di)
			{
			    vect.push_back(di);
			    if (ss.peek() == ',')
			        ss.ignore();
			}

			xval = vect[0];
			yval = vect[1];
			theta = vect[2];

		};
		waypoint(double x = 0.0, double y = 0.0, double t = 0.0) {
			xval = x;
			yval = y;
			theta = t;
		};
		waypoint(){
			xval = 0.0;
			yval = 0.0;
			theta = 0.0;
		};

		double getX();
		double getY();
		double getTheta();
		void setX(double val);
		void setY(double val);
		void setTheta(double val);
		string toString();
};


}

#endif /* INCLUDE_MODEL_WAYPOINT_H_ */
