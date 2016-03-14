
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include "../../include/controllers/MotorController.h"
#include "../../include/globals.h"
#include "../include/system/console.h"

using namespace std;

namespace blaze {

MotorController::MotorController(string m1_pwm_pin, string m2_pwm_pin, GPIO *gpio_m1_dir, GPIO *gpio_m2_dir) {
	// TODO Auto-generated constructor stub

	// M1 is probably 46 - left
	// M2 is probably 45 - right
	this->gpio_m1_dir = gpio_m1_dir;
	this->gpio_m2_dir = gpio_m2_dir;
	this->m1 = new PWM(m1_pwm_pin);
	this->m2 = new PWM(m2_pwm_pin);

	this->gpio_m1_dir->setDirection(GPIO::OUTPUT);
	this->gpio_m2_dir->setDirection(GPIO::OUTPUT);

	this->init();
	return;
}
MotorController::MotorController(string m1_pwm_pin, string m2_pwm_pin, int gpio_m1_dir, int gpio_m2_dir) {
	// TODO Auto-generated constructor stub
	console::debug("motor controller constructor: M1 PWM: " + m1_pwm_pin + + " M1 DIR: " + to_string(gpio_m1_dir) + " M2 PWM: " + m2_pwm_pin + " M2 DIR: " + to_string(gpio_m2_dir));
	this->gpio_m1_dir = new GPIO(gpio_m1_dir);
	this->gpio_m2_dir = new GPIO(gpio_m2_dir);

	this->m1 = new PWM(m1_pwm_pin);
	this->m2 = new PWM(m2_pwm_pin);

	this->init();
	return;
}


void MotorController::init(){
	console::debug("init motor controller");
	this->m1->setPolarity(PWM::POLARITY::ACTIVE_HIGH);
	this->m2->setPolarity(PWM::POLARITY::ACTIVE_HIGH);

	console::debug("set m1dir as output");
	//this->m1->stop();
	//this->m2->stop();

	this->gpio_m1_dir->setDirection(GPIO::OUTPUT);
	console::debug("set m2dir as output");
	this->gpio_m2_dir->setDirection(GPIO::OUTPUT);
	console::debug("set m2dir as output completed");
	return;
}

void MotorController::engage(int motorID, SPEED speed){

	PWM *p ;
	if(motorID == 1){
		p = this->m1;
	}else{
		p = this->m2;
	}

	p->run();

}

void MotorController::brake(int motorID){

	if(motorID == 1){
		this->breakMotor = this->m1;
	}else{
		this->breakMotor = this->m2;
	}

	this->threadedStepPeriod=100; //if you ever wanted to stop over time...
	this->threadRunning = true;
    if(pthread_create(&this->thread, NULL, &threadedBreak, static_cast<void*>(this))){
    	//console->error("MotorController: Failed to create the stepping thread");
    	this->threadRunning = false;
    	return;
    }


}

void MotorController::slower(int motorID){

}

void MotorController::faster(int motorID){

}

//dual motor

void MotorController::forward(){
	console::debug("MotorController: forward");
	this->threadedStepNumber = 5;


	unsigned int v = 100000;
	this->m1->setDutyCycle(v);
	this->m2->setDutyCycle(v);

	this->m1->run();
	this->m2->run();

}

void MotorController::forward(SPEED rate){
	this->threadedStepNumber = 5;

	switch(rate){
	case SPEED::EIGHTH:
		this->new_velocity = MOTOR_MAX_DUTY / 8;
		break;
	case SPEED::QUARTER:
		this->new_velocity = MOTOR_MAX_DUTY / 4;
		break;
	case SPEED::THREE_EIGHTH:
		this->new_velocity = MOTOR_MAX_DUTY * 0.375;
		break;
	case SPEED::HALF:
		this->new_velocity = MOTOR_MAX_DUTY / 2;
		break;
	case SPEED::FIVE_EIGHTH:
		this->new_velocity = MOTOR_MAX_DUTY * 0.625;
		break;
	case SPEED::THREE_QUARTER:
		this->new_velocity = MOTOR_MAX_DUTY * 0.75;
		break;
	case SPEED::SEVEN_EIGHTH:
		this->new_velocity = MOTOR_MAX_DUTY * 0.875;
		break;
	case SPEED::FULL:
		this->new_velocity = MOTOR_MAX_DUTY;
		break;
	}

	unsigned int dutym1 = this->m1->getDutyCycle();
	unsigned int dutym2 = this->m2->getDutyCycle();



	//cout << " max duty: " +  to_string(MOTOR_MAX_DUTY);
	//cout << " new vel: " + to_string(this->new_velocity) << endl;
	//split the velocity into 5 step up items.
	this->velocity_step = this->new_velocity / 5;


	this->threadedStepPeriod=100; //if you ever wanted to stop over time...
	this->threadRunning = true;

	if(pthread_create(&this->thread, NULL, &threadedForward, static_cast<void*>(this))){
		perror("StepperMotor: Failed to create the stepping thread");
		cout << "error" << endl;
		//console->error("MotorController: Failed to create the stepping thread");
		this->threadRunning = false;
		return;
	}

}


void MotorController::backward(){

}
void MotorController::backward(SPEED rate){

}

bool MotorController::isBusy(){
	return this->threadRunning;
}


void MotorController::stop(){

	this->threadedStepNumber = 5;
	this->m1_v_reduce = this->m1->getDutyCycle()/this->threadedStepNumber;
	this->m2_v_reduce = this->m1->getDutyCycle()/this->threadedStepNumber;

	this->threadedStepPeriod=100; //if you ever wanted to stop over time...
	this->threadRunning = true;

    if(pthread_create(&this->thread, NULL, &threadedStop, static_cast<void*>(this))){
    	perror("StepperMotor: Failed to create the stepping thread");
    	cout << "error" << endl;
    	//console->error("MotorController: Failed to create the stepping thread");
    	this->threadRunning = false;
    	return;
    }
}


void* threadedForward(void *value){
	MotorController *mc = static_cast<MotorController*>(value);
	//cout << " forward" << endl;
	unsigned int dutym1 = mc->m1->getDutyCycle();
	unsigned int dutym2 = mc->m2->getDutyCycle();
	unsigned int new_velocity = 0;
	bool isStepUp = true;

	if(dutym1 > mc->new_velocity){
		//step down
		isStepUp = false;
		new_velocity = mc->new_velocity;
	}

	mc->m1->run();
	mc->m2->run();

	while(mc->threadRunning){
		//mc->correctionFactor

		if(isStepUp){
			new_velocity = new_velocity + mc->velocity_step;
			if(dutym1 < new_velocity){
				mc->m1->setDutyCycle(new_velocity);
				dutym1 = new_velocity;
			}

			if(dutym2 < new_velocity){
				mc->m2->setDutyCycle(new_velocity);
				dutym2 = new_velocity;
			}
		}else{
			new_velocity = new_velocity - mc->velocity_step;
				mc->m1->setDutyCycle(new_velocity);
				dutym1 = new_velocity;

				mc->m2->setDutyCycle(new_velocity);
				dutym2 = new_velocity;
		}



		usleep(mc->threadedStepPeriod * 1000);  // convert from ms to us
		if(mc->threadedStepNumber>0) mc->threadedStepNumber--;
		if(mc->threadedStepNumber==0) mc->threadRunning = false;
	}
	return 0;
}


void* threadedBreak(void *value){
	MotorController *mc = static_cast<MotorController*>(value);

	int v_reduce = mc->breakMotor->getDutyCycle()/mc->threadedStepNumber;
	mc->breakMotor->setDutyCycle(mc->breakMotor->getDutyCycle() - v_reduce);

	while(mc->threadRunning){

		mc->breakMotor->setDutyCycle(mc->m1->getDutyCycle() - v_reduce);

		usleep(mc->threadedStepPeriod * 1000);  // convert from ms to us
		if(mc->threadedStepNumber>0) mc->threadedStepNumber--;
		if(mc->threadedStepNumber==0) mc->threadRunning = false;
	}
	return 0;

}

void* threadedStop(void *value){
	MotorController *mc = static_cast<MotorController*>(value);
	while(mc->threadRunning){
		mc->m1->setDutyCycle(mc->m1->getDutyCycle() - mc->m1_v_reduce);
		mc->m2->setDutyCycle(mc->m2->getDutyCycle() - mc->m2_v_reduce);

		usleep(mc->threadedStepPeriod * 1000);  // convert from ms to us
		if(mc->threadedStepNumber>0) mc->threadedStepNumber--;
		if(mc->threadedStepNumber==0) mc->threadRunning = false;
	}
	return 0;
}



} /* namespace blaze */


















/*
 * TODO: Throw away
 * 		 DCMotor dcm(new PWM("pwm_test_P9_42.12"), 116); //will export GPIO116
		   dcm.setDirection(DCMotor::ANTICLOCKWISE);
		   dcm.setSpeedPercent(50.0f);   //make it clear that a float is passed
		   dcm.go();
		   cout << "Rotating Anti-clockwise at 50% speed" << endl;
		   usleep(5000000);    //sleep for 5 seconds
		   dcm.reverseDirection();
		   cout << "Rotating clockwise at 50% speed" << endl;
		   usleep(5000000);
		   dcm.setSpeedPercent(100.0f);
		   cout << "Rotating clockwise at 100% speed" << endl;
		   usleep(5000000);
		   dcm.stop();
		   cout << "End of EBB DC Motor Example" << endl;
 */
