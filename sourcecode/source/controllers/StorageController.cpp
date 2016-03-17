/*
rffrt * StorageController.cpp
 *
 *  Created on: Oct 4, 2015
 *      Author: Peter
 */

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include "../../include/controllers/StorageController.h"
#include "../../include/globals.h"
#include "../include/system/console.h"

using namespace std;

namespace blaze {

StorageController::StorageController(string pwmName) {
	// TODO Auto-generated constructor stub
	this->pinName = pwmName;

}

StorageController::~StorageController() {
	// TODO Auto-generated destructor stub
}

void StorageController::setDrivePostion(){
	  PWM pwm(this->pinName);  // P9_42 MUST be loaded as a slot before use
	   //pwm.setPeriod(500000);         // Set the period in ns
	   unsigned int i;
	   i = 350000;
	   pwm.setDutyCycle(i);
	   pwm.setPolarity(PWM::ACTIVE_HIGH);  // using active low PWM
	   pwm.run();                     // start the PWM output

}
void StorageController::setDeliveryPosition(){
	  PWM pwm(this->pinName);  // P9_42 MUST be loaded as a slot before use
	   unsigned int i;
	   i = 400000;
	   pwm.setDutyCycle(i);
	   pwm.setPolarity(PWM::ACTIVE_HIGH);  // using active low PWM
	   pwm.run();
}
void StorageController::setLoadPosition(){
	  PWM pwm(this->pinName);  // P9_42 MUST be loaded as a slot before use
	   unsigned int i;
	   i = 400000;
	   pwm.setDutyCycle(i);
	   pwm.setPolarity(PWM::ACTIVE_HIGH);  // using active low PWM
	   pwm.run();
}
void StorageController::setRestPosition(){
	 PWM pwm(this->pinName);  // P9_42 MUST be loaded as a slot before use
	   pwm.setDutyCycle(ZERO);
	   pwm.setPolarity(PWM::ACTIVE_HIGH);  // using active low PWM
	   pwm.stop();
}


} /* namespace blaze */
