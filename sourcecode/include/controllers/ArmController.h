/*
 * ArmController.h
 *
 *  Created on: Oct 4, 2015
 *      Author: Peter
 */

#ifndef INCLUDE_CONTROLLERS_ARMCONTROLLER_H_
#define INCLUDE_CONTROLLERS_ARMCONTROLLER_H_

#include "../io/DynaMX12WServo.h"
#include "../io/StepperMotor.h"

namespace blaze {
	/*
	 * The Image Controller is responsible for interacting with the
	 * camera, and processing images.
	 */
	class ArmController {
	public:
		ArmController(StepperMotor *sc);
		virtual ~ArmController();

		/**
		 * Arm Initialization and Control
		 */
		void init();
		void reset();
		void PrintCommStatus(int CommStatus);
		void PrintErrorCode();

		long baudRate;
		bool isConnected;
		bool isMoving;
		bool moveCompleted;

		DynaMX12WServo servo20; // MX
		DynaMX12WServo servo21;
		DynaMX12WServo servo22;
		DynaMX12WServo servo23;
		DynaMX12WServo servo24;
		DynaMX12WServo servo25;	// MX

		void stopNow();


		void startup();


		/**
		 * Servo control system
		 */
		void printServoIds();


		/**
		 * Stepper Control system
		 */
		void moveStepper();



	private:
		StepperMotor *stepper;

	};

}



#endif /* INCLUDE_CONTROLLERS_ARMCONTROLLER_H_ */


/*
 *  cout << "*** Start of Motor Test" << endl;
		 StepperMotor motor1(44, 26, 46, 65, 30, 61, 113, 20, 7, 60, 200);


		 	//In case the motor SLP (sleep) pin is low
		 	motor1.wake();

		 	cout << "*** Start of Motor Test" << endl;
		 	cout << "*** Rotating - Forward 360 degrees (full step)" << endl;
		 	//forward single step
		 	motor1.rotate(360);
		 	sleep(1);	//sleep for 1 second between each test stage

		 	//back half step
		 	cout << "*** Rotating - Reverse 360 degrees (half step)" << endl;
		 	motor1.setStepMode(StepperMotor::STEP_HALF);
		 	motor1.reverseDirection();
		 	motor1.rotate(360);
		 	sleep(1);

		 	cout << "*** Set speed to 120rpm" << endl;
		 	motor1.setSpeed(120);

		 	//forward quarter step
		 	cout << "*** Rotating - Forward 360 degrees (quarter step)" << endl;
		 	motor1.setStepMode(StepperMotor::STEP_QUARTER);
		 	motor1.reverseDirection();
		 	motor1.rotate(360);
		 	sleep(1);

		 	//reverse eight step
		 	cout << "*** Rotating - Reverse 360 degrees (eight step)" << endl;
		 	motor1.setStepMode(StepperMotor::STEP_EIGHT);
		 	motor1.rotate(-360);


		 	//reverse eight step
			cout << "*** Rotating - Reverse 360 degrees (16th step)" << endl;
			motor1.setStepMode(StepperMotor::STEP_SIXTEEN);
			motor1.rotate(360);

		 	cout << "*** Sleep the motor for 10 seconds" << endl;
		 	//Sleep the EasyDriver controller
		 	motor1.sleep();  //easy to move motor shaft while sleep is enabled
		 					 //unexport of SLP pin can set this low, meaning shaft
		 					 //torque is high when finished
		 	sleep(10);
		 	//motor1.wake(); not necessary in this case as destructor of motor1 calls unexport

		 	cout << "*** Motor Test Finished" << endl;

		 	//motor1 object destroyed now (goes out of scope)

 */

