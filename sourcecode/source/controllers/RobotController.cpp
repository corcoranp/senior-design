/*
 * RobotController.cpp
 *
 * By: Peter Corcoran
 * Copyright: © 2015
 *
 * Jun 27, 2015 - Initial Coding
 *
 * This file is used as the main controller system of
 * The responsibility of this class is to control system operations
 *
 * 1. System Check
 * 2. Hardware Check
 * 3. Sensor Check
 * 4. Boot State Check
 * 5. Running each port delivery
 *
 */
#include <iostream>
#include <map>
#include <string>

#include "../../include/system/console.h"
#include "../../include/controllers/RobotController.h"



using namespace std;
using namespace blaze;

bool RobotController::m_isRunning = true;
starter *RobotController::control_buttons = 0;
indicator *RobotController::state_display = 0;
MotorController *RobotController::motorControl = 0;
StepperMotor *RobotController::stepperControl=0;
ArmController *RobotController::armControl=0;
StorageController *RobotController::storageControl = 0;
ImagingController *RobotController::imageControl = 0;
NavigationController *RobotController::navControl = 0;

/*
 * Thread function that will execute when the thread is created
 * it passes and receives data by void pointers
 */
void *RobotController::entry(void *args){
	RobotController *thisClass = (RobotController *)args; //cast the data passed to pointer to sysops

	thisClass->start();
}

bool RobotController::getIsRunning(){
	return RobotController::m_isRunning;
}
void RobotController::setIsRunning(bool val){
	m_isRunning = val;
}

/*
 * Start function runs at boot to handle check events....
 */
void RobotController::start(){
	console::debug("RobotController Started");
	//Thread that runs the main robot functions.
	console::debug("Setup Display buttons");

	state_display = new indicator( gpio_led1,  gpio_led2,  gpio_led3,  gpio_led4 );

	console::debug("Setup up Control buttons");
	control_buttons = new starter( gpio_start_switch,  gpio_stop_switch);




	console::debug("Hello, My name is BLAZE...let me take you on a journey...");
	console::debug("Press the start button when ready to begin...");

	while(!control_buttons->isStartPressed()){
		 //wait until start is pressed...
		 usleep(100000);
		 state_display->blink(100,indicator::LED::TOP);
	 }

	console::debug("Let's begin.");
	//state_display.started();
	state_display->on(2);


	console::debug("Setup Motor Controller");

	motorControl = new MotorController(M1_PWM, M2_PWM, gpio_motor1_dir, gpio_motor2_dir );
	console::debug("RC: MC-> All Stop");
	motorControl->stop();

	//Zero Robot....
	console::debug("Setup Navigation Controller");
	this->navControl = new NavigationController(motorControl);


	console::debug("Setup Stepper Motor");
	stepperControl = new StepperMotor(gpio_stepper_ms1,gpio_stepper_ms2,gpio_stepper_ms3,gpio_stepper_step,gpio_stepper_sleep, gpio_stepper_dir, gpio_stepper_en, gpio_stepper_lsw, gpio_stepper_rsw, 60, 200);
	stepperControl->disable();

	console::debug("Setup Arm Controller");
	//armControl = new ArmController(stepperControl);

	console::debug("Setup Storage Controller");
	storageControl = new StorageController(STORAGE_PWM);
	storageControl->setRestPosition();



	//double x = navControl->getPosition(Face::Front, DistType::Theta);
	//console::debug( "NC.getPosition = " +  to_string(x) );

	/**********************************************************************************
	 *
	 * Execusion code....
	 *
	 */
	state_display->on(3);
	PortConfig pc = navControl->determinePort();



	if(pc == PortConfig::A){
		//turn right
		console::debug("Current Port Config: A");
	}
	if(pc == PortConfig::B){
		//turn left
		console::debug("Current Port Config: B");
	}



	navControl->moveUntil(300, MOVEMENT::FORWARD);

	storageControl->setDrivePostion();

	navControl->turn(0,0);




	while(0){
		navControl->moveUntil(300, MOVEMENT::FORWARD);
		navControl->turn(0,0);
	}

	if(pc == PortConfig::A){
		//turn right
		console::debug("Current Port Config: A");
		//navControl->move(WALL_FOLLOWING::LEFT, 300, 0,200);
	}
	if(pc == PortConfig::B){
		//turn left
		console::debug("Current Port Config: B");
		//navControl->move(WALL_FOLLOWING::RIGHT, 300, 0,200);
	}



	//armControl->startup();





	//**********************************************************************************
	bool restart = false;
	console::debug("You can now do things and when finished, press the stop button.");
	 while(!control_buttons->isStopPressed()){
		 //process until stop is pressed...
		 usleep(1000);
		/* if(control_buttons->isStopPressed()){
			 restart = true;
			 state_display->blink(100,indicator::LED::THRID);
		 }*/

	 }
	 state_display->off(1);
	 state_display->off(2);
	 state_display->off(3);
	 state_display->off(4);

	 motorControl->stopNow();
	 //armControl->stopNow();
	 navControl->stopNow();
	 storageControl->setRestPosition();


	 if(restart){
	 //restart...
		 system("sudo ./start.sh");
	 }


/*
	 console::debug("System is not ready to be restarted...");
	 while(!control_buttons.isStopPressed()){
		 //process until stop is pressed...


		 usleep(100000);
		 state_display.blink(100,indicator::LED::TOP);
		 state_display.blink(100,indicator::LED::SECOND);
		 state_display.blink(100,indicator::LED::THRID);
		 state_display.blink(100,indicator::LED::BOTTOM);
	 }

*/

	 //%%%%%%%%%%%%%%%%%%%%%


	//StorageController sc(STORAGE_PWM);
	//sc.setRestPosition();





/*
	console::debug("motor controller created");
	mc.forward(SPEED::SEVEN_EIGHTH);
	while(mc.isBusy()){
		usleep(10000);
	}

	usleep (2000000);

	mc.stop();

	while(mc.isBusy()){
		usleep(10000);
	}
*/

	//usleep(3000000);
	//sc.setDrivePostion();
	//usleep(3000000);
	//sc.setRestPosition();

	/*
	this->motorControl = new MotorController(M1_PWM, M2_PWM, gpio_motor1_dir, gpio_motor2_dir);

	console::debug("motor controller created");
	this->motorControl->forward();

	usleep(1000000);

	this->motorControl->stop();
*/
	//. Identify Port @ startup
/*
	this->currentPort.portcfg = NavigationController::determinePort();
	if(this->currentPort.portcfg == PortConfig::A){

		console::info("Port A Detected");
	}
	if(this->currentPort.portcfg == PortConfig::B){

		console::info("Port B Detected");
	}
	if(this->currentPort.portcfg == PortConfig::UNDEFINED){
		console::error("Port Undetected");
		state_display.errorState();
	}

*/
	//. Navigate through tunnel





	//. Solve section A
	//. Solve section C
	//. Solve section B

	 /*if(!LIDAR_ENABLED){
		 char *port = new char[ LIDAR_PORT.length() + 1];
		 strcpy(port,  LIDAR_PORT.c_str());
		 lidarIO lidar(port);
		 lidar.disable();
	 }*/


	console::debug("RobotController Ended");
	RobotController::setIsRunning(false);
}



