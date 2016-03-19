/*
 * indicator.cpp
 *
 *  Created on: Mar 11, 2016
 *      Author: Peter
 */

#include "../../include/system/indicator.h"
#include <unistd.h>
using namespace std;

namespace blaze {

/**
 * @class indicator
 * @brief
 */
indicator::indicator(GPIO *gpio_led1, GPIO *gpio_led2, GPIO *gpio_led3, GPIO *gpio_led4){

	this->gpio_led1 = gpio_led1;
	this->gpio_led2 = gpio_led2;
	this->gpio_led3 = gpio_led3;
	this->gpio_led4 = gpio_led4;


}
indicator::indicator(int gpio_led1, int gpio_led2, int gpio_led3, int gpio_led4){
	this->gpio_led1 = new GPIO(gpio_led1);
	this->gpio_led2 = new GPIO(gpio_led2);
	this->gpio_led3 = new GPIO(gpio_led3);
	this->gpio_led4 = new GPIO(gpio_led4);

	/*this->gpio_led1->setDirection(GPIO::OUTPUT);
	this->gpio_led2->setDirection(GPIO::OUTPUT);
	this->gpio_led3->setDirection(GPIO::OUTPUT);
	this->gpio_led4->setDirection(GPIO::OUTPUT);
*/
	//this->gpio_led1->setValue(GPIO::HIGH);

	//HIGH is OFF
	//LOW is ON

}

void indicator::blink(int ms, LED led){

	switch(led){
	case LED::TOP:
		this->gpio_led1->setValue(GPIO::HIGH);
		usleep(ms * 1000);
		this->gpio_led1->setValue(GPIO::LOW);
		break;
	case LED::SECOND:
		this->gpio_led2->setValue(GPIO::HIGH);
		usleep(ms * 1000);
		this->gpio_led2->setValue(GPIO::LOW);
		break;
	case LED::THRID:
		this->gpio_led3->setValue(GPIO::LOW);
		usleep(ms * 1000);
		this->gpio_led3->setValue(GPIO::HIGH);
		break;
	case LED::BOTTOM:
		this->gpio_led4->setValue(GPIO::LOW);
		usleep(ms * 1000);
		this->gpio_led4->setValue(GPIO::HIGH);
		break;
	}

}
void indicator::started(){
	this->gpio_led1->setValue(GPIO::LOW);
	this->gpio_led2->setValue(GPIO::HIGH);
	this->gpio_led3->setValue(GPIO::HIGH);
	this->gpio_led4->setValue(GPIO::HIGH);
}
void indicator::motor1running(bool isrunning){
	if(isrunning){
		this->gpio_led3->setValue(GPIO::HIGH);
	}else{
		this->gpio_led3->setValue(GPIO::LOW);
	}
}
void indicator::motor2running(bool isrunning){
	if(isrunning){
		this->gpio_led4->setValue(GPIO::HIGH);
	}else{
		this->gpio_led4->setValue(GPIO::LOW);
	}
}
void indicator::errorState(){
	this->gpio_led1->setValue(GPIO::HIGH);
	this->gpio_led2->setValue(GPIO::LOW);
	this->gpio_led3->setValue(GPIO::LOW);
	this->gpio_led4->setValue(GPIO::HIGH);
}

void indicator::on(int light){
	switch(light){
	case 1: 	this->gpio_led1->setValue(GPIO::LOW); break;
	case 2: 	this->gpio_led1->setValue(GPIO::LOW); break;
	case 3: 	this->gpio_led1->setValue(GPIO::LOW); break;
	case 4: 	this->gpio_led1->setValue(GPIO::LOW); break;
	}
}
void indicator::off(int light){
	switch(light){
	case 1: 	this->gpio_led1->setValue(GPIO::HIGH); break;
	case 2: 	this->gpio_led1->setValue(GPIO::HIGH); break;
	case 3: 	this->gpio_led1->setValue(GPIO::HIGH); break;
	case 4: 	this->gpio_led1->setValue(GPIO::HIGH); break;
	}
}

indicator::~indicator() {
	// TODO Auto-generated destructor stub
}

} /* namespace blaze */
