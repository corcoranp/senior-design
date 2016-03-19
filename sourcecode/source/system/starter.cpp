/*
 * starter.cpp
 *
 *  Created on: Mar 11, 2016
 *      Author: Peter
 */

#include "../../include/system/starter.h"
#include <unistd.h>
namespace blaze {

starter::starter(GPIO *gpio_start, GPIO *gpio_stop){
	this->gpio_start = gpio_start;
	this->gpio_stop = gpio_stop;
}
starter::starter(int gpio_start, int gpio_stop){
	this->gpio_start = new GPIO(gpio_start);
	//usleep(100);
	this->gpio_stop = new GPIO(gpio_stop);
	//usleep(100);
	this->gpio_start->setDirection(GPIO::INPUT);
	this->gpio_stop->setDirection(GPIO::INPUT);
}

bool starter::isStartPressed(){
	if(this->gpio_start->getValue() == GPIO::LOW){
		return true;
	}
	return false;
}
bool starter::isStopPressed(){
	if(this->gpio_stop->getValue() == GPIO::LOW){
		return true;
	}
	return false;
}

starter::~starter() {
	// TODO Auto-generated destructor stub
}

} /* namespace blaze */
