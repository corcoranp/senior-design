/*
 * starter.cpp
 *
 *  Created on: Mar 11, 2016
 *      Author: Peter
 */

#include "starter.h"

namespace blaze {

starter::starter(GPIO *gpio_start, GPIO *gpio_stop){
	this->gpio_start = gpio_start;
	this->gpio_stop = gpio_stop;
}
starter::starter(int gpio_start, int gpio_stop){
	this->gpio_start = new GPIO(gpio_start);
	this->gpio_stop = new GPIO(gpio_stop);

	this->gpio_start->setDirection(GPIO::INPUT);
	this->gpio_stop->setDirection(GPIO::INPUT);
}

bool starter::requestStart(){
	if(this->gpio_start->getValue() == GPIO::HIGH){
		return true;
	}
	return false;
}
bool starter::requestStop(){
	if(this->gpio_stop->getValue() == GPIO::HIGH){
		return true;
	}
	return false;
}

starter::~starter() {
	// TODO Auto-generated destructor stub
}

} /* namespace blaze */
