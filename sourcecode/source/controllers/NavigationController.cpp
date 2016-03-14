/*
 * NavigationController.cpp
 *
 *  Created on: Mar 12, 2016
 *      Author: Peter
 */

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include "../../include/controllers/NavigationController.h"
#include "../../include/io/lidarIO.h"
#include "../../include/globals.h"
#include "../include/system/console.h"

using namespace std;

namespace blaze {

NavigationController::NavigationController() {
	// TODO Auto-generated constructor stub

}

NavigationController::~NavigationController() {
	// TODO Auto-generated destructor stub
}


PortConfig NavigationController::determinePort(){
	console::debug("NavigationController::determinePort() start");

	try {
		double data[370] ;
		int left_max; int right_max; int i;
		left_max = 0;
		right_max = 0;

		lidarIO lidar(LIDAR_PORT);
		lidar.connect();
		console::debug("LIDAR: get Data");
		lidar.getData(lidar.lidarFileDescriptor, data);

		for (i=17; i<=50; i++){
			if(data[i] > left_max) {
				left_max = data[i];
			}
		}

		for(i=285; i<=343; i++){
			if(data[i] > right_max){
				right_max = data[i];
			}
		}

		cout << "left max " + to_string(left_max) << endl;
		cout << "right max: " + to_string(right_max) << endl;


		if(left_max > right_max){
			return PortConfig::B;
		}
		if( right_max > left_max ){
			return PortConfig::A;
		}
	}catch (int err){
		console::error("Error occured in NavigationController: " + to_string(err));
	}

	return PortConfig::UNDEFINED;
}

void NavigationController::moveToFirstPosition(){


}




} /* namespace blaze */
