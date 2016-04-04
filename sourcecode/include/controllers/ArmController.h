/*
 * ArmController.h
 *
 *  Created on: Oct 4, 2015
 *      Author: Peter
 */

#ifndef INCLUDE_CONTROLLERS_ARMCONTROLLER_H_
#define INCLUDE_CONTROLLERS_ARMCONTROLLER_H_

#include "../io/DynaServo.h"
#include "../io/DynaController.h"
#include "../io/StepperMotor.h"
#include "../Arm/ArmConstraint.h"
#include <iostream>
#include <string>
#include <cmath>

using namespace dyn;
using namespace std;

//Define servo id's
#define BASE_SERVO_ID 20
#define LINK_1_SERVO_ID 21
#define LINK_2_SERVO_ID 22
#define LINK_3_SERVO_ID 23
#define ROTATE_WRIST_SERVO_ID 24
#define GRIPPER_SERVO_ID 25

/////////////Define servo Constraints/////////////////
#define BASE_MIN_ANG 66.0
#define BASE_MAX_ANG 360.0
#define LINK_1_MIN_ANG 95.0
#define LINK_1_MAX_ANG 239.5
#define LINK_2_MIN_ANG 54.0
#define LINK_2_MAX_ANG 253.50
#define LINK_3_MIN_ANG 46.0
#define LINK_3_MAX_ANG 239.5
#define ROTATE_MIN_ANG 86.0
#define ROTATE_MAX_ANG 320.0
#define GRIPPER_MIN_ANG 104.0
#define GRIPPER_MAX_ANG 140.0

/////////////Define servo Speed(RPM)///////////////////
#define BASE_SPEED 11.0
#define LINK_1_SPEED 11.0
#define LINK_2_SPEED 11.0
#define LINK_3_SPEED 11.0
#define ROTATE_SPEED 15.0
#define GRIPPER_SPEED 15.0

/////////////Define internal check values//////////////
#define ANGLE_PRESISION 1.0 //angle presision in degrees
#define ARM_STEP_ANGLE_SIZE 5.0 //step size in degrees
#define ARM_SERVO_ANGLE_DST_PRES 4.0 //step size in degrees
#define SERVO_STEP_DIVIDER_HIGH 3.0
#define SERVO_STEP_DIVIDER_LOW 5.0

/////////////Define Stepper values//////////////

namespace blaze {
	/*
	 * The Image Controller is responsible for interacting with the
	 * camera, and processing images.
	 */
	class ArmController {
	public:

		typedef struct ServoStatus_t {
			double temperatureF;
			double temperatureC;
			double voltage;
			double angle;
			bool isMoving;
			bool alarmShutdown;
			DynaServo::alarm alarmCondition;
		} ServoStatus;

		ArmController(StepperMotor *sc) throw (string);
		virtual ~ArmController();

		/**
		 * Arm Initialization and Control
		 */
		void init();
		void reset();

		bool isConnected();

		bool isMoving();

		bool moveCompleted();

		void stopNow();


		void startup();


		/**
		 * Stepper Control system
		 */
		void moveStepper(int mm);

		void homeStepper();

		bool isStepperHome();

		bool isStepperAtLimit();

		int getStepperDistance();


		///////////////////////Basic manual set angle Functions///////////
		double getBaseAngle();
		void setBaseAngle(double angle) throw (string);
		double getLink1Angle();
		void setLink1Angle(double angle) throw (string);
		double getLink2Angle();
		void setLink2Angle(double angle) throw (string);
		double getLink3Angle();
		void setLink3Angle(double angle) throw (string);
		double getRotateAngle();
		void setRotateAngle(double angle) throw (string);
		double getGripperAngle();
		void setGripperAngle(double angle) throw (string);
		//////////////////////////////////////////////////////////////////
		//////////////////////Get the current status of the servo/////////
		ServoStatus getBaseStatus();
		ServoStatus getLink1Status();
		ServoStatus getLink2Status();
		ServoStatus getLink3Status();
		ServoStatus getRotateStatus();
		ServoStatus getGripperStatus();
		//////////////////////////////////////////////////////////////////

		//Zone A
		void zoneATopBlock();
		void zoneABottomBlock();
		void zoneAReset();

		//Zone B
		void zoneBTopBlockLong();
		void zoneBBottomBlockLong();
		void zoneBTopBlockShortFront();
		void zoneBBottomBlockShortFront();
		void zoneBTopBlockShortBack();
		void zoneBBottomBlockShortBack();
		void zoneBReset();
		void zoneBResetShort();

		//Zone C
		void zoneCTopBlock();
		void zoneCBottomBlock();
		void zoneCReset();



	private:
		StepperMotor *stepper;
		ArmConstraint* constriants;
		DynaController* controller;
		DynaServo* base;
		DynaServo* link1;
		DynaServo* link2;
		DynaServo* link3;
		DynaServo* rotate;
		DynaServo* gripper;
		bool isMove;
		bool moveComplete;
		int stepperLocation;


		//Private functions
		/**
		 * Get the next angle step in the direction of the dst.
		 */
		double angleStep(double loc, double dst);
		/**
		 * Get the calculated step size.
		 */
		double calculateStepSize(double loc, double dst);
		/**
		 * Get the angle difference between the angles.
		 */
		double angleDiff(double loc, double dst);
		/**
		 * Check if angles are equal in range of pres.
		 */
		bool angleEqaul(double a, double b, double pres);
		/**
		 * Check if a is greater then b in range of pres.
		 */
		bool angleGreater(double a, double b, double pres);
		/**
		 * Check if a is less then b in range of press.
		 */
		bool angleLess(double a, double b, double pres);

		bool moveServo(DynaServo *s, double step);

		bool servoReachedLocation(DynaServo *s, double dst, double pres);

		bool moveToLocation(double bs, double lk1, double lk2, double lk3, double rt);

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

