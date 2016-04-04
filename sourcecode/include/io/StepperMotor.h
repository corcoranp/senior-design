/*
 * StepperMotor.h  Created on: 13 Jun 2014
 * Copyright (c) 2014 Derek Molloy (www.derekmolloy.ie)
 * Made available for the book "Exploring BeagleBone" 
 * See: www.exploringbeaglebone.com
 * Licensed under the EUPL V.1.1
 *
 * This Software is provided to You under the terms of the European 
 * Union Public License (the "EUPL") version 1.1 as published by the 
 * European Union. Any use of this Software, other than as authorized 
 * under this License is strictly prohibited (to the extent such use 
 * is covered by a right of the copyright holder of this Software).
 *
 * This Software is provided under the License on an "AS IS" basis and 
 * without warranties of any kind concerning the Software, including 
 * without limitation merchantability, fitness for a particular purpose, 
 * absence of defects or errors, accuracy, and non-infringement of 
 * intellectual property rights other than copyright. This disclaimer 
 * of warranty is an essential part of the License and a condition for 
 * the grant of any rights to this Software.
 *
 * For more details, see http://www.derekmolloy.ie/
 */

#ifndef STEPPERMOTOR_H_
#define STEPPERMOTOR_H_
#include "../../include/io/gpio.h"

namespace blaze {

/**
 * @class StepperMotor
 * @brief A class to control a stepper motor using a motor driver board, such as the
 * Easy Driver board, or compatible. The class uses five GPIOs to control each motor.
 */
class StepperMotor {

public:
	enum STEP_MODE { STEP_FULL, STEP_HALF, STEP_QUARTER, STEP_EIGHT, STEP_SIXTEEN };
	enum DIRECTION { CLOCKWISE, ANTICLOCKWISE };
	enum HOME_POSITION { LEFT, RIGHT};

private:
	// The GPIO pins MS1, MS2 (Microstepping options), STEP (The low->high step)
	// SLP (Sleep - active low) and DIR (Direction)


   GPIO *gpio_MS1, *gpio_MS2, *gpio_MS3, *gpio_STEP, *gpio_SLP, *gpio_DIR, *gpio_EN, *gpio_LSW, *gpio_RSW;
   unsigned int uSecDelay;
   DIRECTION direction;
   int delayFactor;      // keep constant rpm even with microstepping
   STEP_MODE stepMode;
   float speed;
   int stepsPerRevolution;
   bool asleep;
   bool enabled; //PMC added
   void init(int speedRPM, int stepsPerRevolution);

public:
   StepperMotor(GPIO *gpio_MS1, GPIO *gpio_MS2, GPIO *gpio_MS3, GPIO *gpio_STEP, GPIO *gpio_SLP,
    			GPIO *gpio_DIR, GPIO *gpio_EN, GPIO *gpio_LSW, GPIO *gpio_RSW, int speedRPM = 60, int stepsPerRevolution = 200);
   StepperMotor(int gpio_MS1, int gpio_MS2, int gpio_MS3, int gpio_STEP, int gpio_SLP,
    			int gpio_DIR, int gpio_EN, int gpio_LSW, int gpio_RSW, int speedRPM = 60, int stepsPerRevolution = 200);

   virtual void  step();
   virtual void  step(int numberOfSteps);
   virtual int   threadedStepForDuration(int numberOfSteps, int duration_ms);
   virtual void  threadedStepCancel() { this->threadRunning = false; }
   virtual void  rotate(float degrees);
   virtual void  setDirection(DIRECTION direction);
   virtual DIRECTION getDirection() { return this->direction; }
   virtual void  reverseDirection();
   virtual void  setStepMode(STEP_MODE mode);
   virtual STEP_MODE getStepMode() { return stepMode; }
   virtual void  setSpeed(float rpm);
   virtual float getSpeed() { return speed; }
   virtual void  setStepsPerRevolution(int steps) { stepsPerRevolution = steps; }
   virtual int   getStepsPerRevolution() { return stepsPerRevolution; }
   virtual void  sleep();
   virtual void  wake();
   virtual void enable();//PMC added
   virtual void disable();//PMC added
   virtual bool isEnabled() {return enabled; } //PMC added
   virtual bool  isAsleep() { return asleep; }
   virtual void setHome(HOME_POSITION position);
   virtual HOME_POSITION getHome();
   virtual bool goHome();
   virtual bool isHome();
   virtual bool isAtEnd();
   virtual bool isAtLimit();
   virtual void checkDirection(int steps);

   virtual ~StepperMotor();

private:
   HOME_POSITION homePosition;
   bool threadRunning;
   pthread_t thread;
   CallbackType callbackFunction;
   int threadedStepPeriod, threadedStepNumber;
   friend void* threadedStep(void *value);
};

void* threadedStep(void *value);

} /* namespace exploringBB */

#endif /* STEPPERMOTOR_H_ */
