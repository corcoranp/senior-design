/*
 * MotorController.h
 *
 *  Created on: Oct 4, 2015
 *      Author: Peter
 */

#ifndef INCLUDE_CONTROLLERS_MOTORCONTROLLER_H_
#define INCLUDE_CONTROLLERS_MOTORCONTROLLER_H_

#include "../../include/globals.h"
#include "../enums.h"
#include "../io/pwm.h"
#include "../io/gpio.h"
#include "../../include/system/indicator.h"

namespace blaze {

class MotorController {
	public:
		bool isEnabled;

		MotorController();
		 MotorController(string m1_pwm_pin, string m2_pwm_pin, GPIO *gpio_m1_dir, GPIO *gpio_m2_dir);
		 MotorController(string m1_pwm_pin, string m2_pwm_pin, int gpio_m1_dir, int gpio_m2_dir);
		 void init();

		double m1_correctionFactor;
		double m2_correctionFactor;

		/*
		* MOTOR CONTROLLER
		*/

		void engage(int motorID, SPEED speed, MOVEMENT direction);
		void brake(int motorID);
		void slower(int motorID);
		void faster(int motorID);



		/*
		* Dual Motor Control
		*/
		void forward();
		void forward(SPEED rate);

		void backward();
		void backward(SPEED rate);

		void stop();
		void stopNow();

		friend void* threadedStop(void *value);
		friend void* threadedBreak(void *value);

		friend void* threadedForward(void *value);

		/**
		 * Thread control
		 */
		bool isBusy();




	private:

		pthread_t thread;

		 int motor1, motor2;
		 string m1_pwmpin, m2_pwmpin;
		 GPIO *gpio_m1_dir, *gpio_m2_dir;
		 PWM *m1, *m2;
		 PWM *breakMotor;


		 unsigned int m1_v_reduce;
		 unsigned int m2_v_reduce;


		 double new_velocity;
		 int velocity_step;
		 bool threadRunning;

		 int threadedStepNumber, threadedStepPeriod;

		 unsigned int getSpeed(SPEED rate, int motorID);

	};

void* threadedForward(void *value);
void* threadedStop(void *value);
void* threadedBreak(void *value);
}
#endif /* INCLUDE_CONTROLLERS_MOTORCONTROLLER_H_ */
