/*
 * pdata.cpp
 *
 *  Created on: Mar 16, 2016
 *      Author: Peter
 */

#include "../../include/model/pdata.h"


	mutex bot_mutex; //global instance of mutex to protect global variable.
	double dist[365]; //global variable

pdata::pdata() {
	// TODO Auto-generated constructor stub

}

pdata::~pdata() {
	// TODO Auto-generated destructor stub
}

void pdata::updateElement(int index, double value){
	//the access to this function is mutually exclusive
	std::lock_guard < std::mutex > guard(bot_mutex);
	dist[index] = value;

}
double pdata::readElement(int index){
	//the access to this function is mutually exclusive:
	std::lock_guard<std::mutex > guard(bot_mutex);
	return dist[index];
}
