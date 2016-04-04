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
#include "pthread.h"
#include <map>
#include <string>

#include "../../include/system/console.h"
#include "../../include/controllers/RobotController.h"



using namespace std;
using namespace blaze;

bool RobotController::m_isRunning = true;
bool RobotController::hasPortBeenFound = false;
PortConfig RobotController::currentPortConfig = PortConfig::UNDEFINED;
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
	//console::debug("ZERO_REF: " + to_string(ZERO_REF));

	//Thread that runs the main robot functions.
	console::debug("Setup Display buttons");
	state_display = new indicator( gpio_led1,  gpio_led2,  gpio_led3,  gpio_led4 );

	console::debug("Setup up Control buttons");
	control_buttons = new starter( gpio_start_switch,  gpio_stop_switch);


	console::debug("Setup Motor Controller");
	motorControl = new MotorController(M1_PWM, M2_PWM, gpio_motor1_dir, gpio_motor2_dir );
	console::debug("RC: MC-> All Stop");
	motorControl->stop();

	//Zero Robot....
	console::debug("Setup Navigation Controller");
	navControl = new NavigationController(motorControl);


	console::debug("Setup Stepper Motor");
	stepperControl = new StepperMotor(gpio_stepper_ms1,gpio_stepper_ms2,gpio_stepper_ms3,gpio_stepper_step,gpio_stepper_sleep, gpio_stepper_dir, gpio_stepper_en, gpio_stepper_lsw, gpio_stepper_rsw, 60, 200);
	stepperControl->disable();

	console::debug("Setup Arm Controller");
	armControl = new ArmController(stepperControl);

	console::debug("Setup Storage Controller");
	storageControl = new StorageController(STORAGE_PWM, DRIVE_POSITION, DELIVERY_POSITION, LOAD_POSITION);
	storageControl->setRestPosition();

	console::debug("Hello, My name is BLAZE...let me take you on a journey...");
	console::debug("Press the start button when ready to begin...");

	while(!control_buttons->isStartPressed()){
		 //wait until start is pressed...
		 usleep(10000);
		 state_display->blink(100,indicator::LED::TOP);
	 }


	/**********************************************************************************
	 *
	 * Execusion code....
	 *
	 */

	state_display->offAll();

	//start position monitor...
	console::debug("RobotController: work queue setup");
	workqueue<measurement*> location_queue;


	console::debug("RobotController: set queue on nav controller");
	NavigationController::m_queue = &location_queue;


	console::debug("RobotController: define navigation thread: navScan");
	pthread_t navScan;
	void* result_scan;
	console::debug("RobotController: Create Thread");

	if(pthread_create(&navScan, NULL, &NavigationController::scan, &navControl)){
		return;
	}
	console::debug("RobotController: Scan Thread Created");
	QUEUING_ENABLED = true;
	SCANNING = true;

	hasPortBeenFound = false;
	console::debug("RobotController: Set port has been found to false");

	//determine what port we are in...
	while(NOT(this->hasPortBeenFound)){
		//console::debug("RC: Check size of queue;");

		if(location_queue.size() > 0){
			console::debug("Item added to Queue" );
			//Pull location data from QUEUE:
			measurement* mea = location_queue.remove();
			currentPortConfig = navControl->determinePort(mea);

			console::debug("Nav current waypoint" + navControl->lastKnownPoint->toString() );

			if(currentPortConfig != PortConfig::UNDEFINED){
				hasPortBeenFound = true;
			}

		}

		rest(100);
	}
	QUEUING_ENABLED = false; //disable measurement queuing

	//wait .1 second
	rest(100);

	console::debug("Solve Tunnel");
	solveTunnel();

	state_display->on(1);
	solveZoneC();

	state_display->on(2);
	scanRail();

	state_display->on(3);
	solveZoneB();

	state_display->on(4);
	solveZoneA();


	dance();


//
//
//
//	//All decisions are made based on location context...so
//	while(true){
//		if(location_queue.size() > 0){
//			//Pull location data from QUEUE:
//			measurement* mea = location_queue.remove();
//
//			//check if port has been found...
//			if(NOT(this->hasPortBeenFound)){
//
//			}
//
//			//Display location information:
//			/*int i;
//			for(i=0; i<=360;i++){
//				cout << to_string(i) + "=" + to_string(mea->distances[i]) + "; " << endl;
//			}*/
//
//			//Display only Angle 0:
//			cout << to_string(0) + "=" + to_string(mea->distances[0]) + "; " << endl;
//			cout << " Theta: " + to_string(mea->getIndexOfMinimumInRange(30, -30)) << endl;
//			cout << " Theta: " + to_string(mea->calculateThetaInRange(30, -30, Face::Right)) << endl;
//
//		}else{
//			//NO ITEMS IN QUEUE...so just wait...
//
//
//			//How long has no items been in q?  Maybe we need to start the localization function over?
//		}
//		usleep(100000);
//	}


	console::debug("Finished the while loop");
	SCANNING = false;


	pthread_join (navScan, &result_scan);






	//**********************************************************************************


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

	 rest(1000); //rest for 1000 ms or 1 second

	delete state_display;
	delete storageControl;
	delete stepperControl;
	delete armControl;
	//delete navControl;
	delete motorControl;


	 if(AUTO_RESTART){
	 //restart...
		 system("sudo ./start.sh");
	 }

	console::debug("RobotController Ended");
	RobotController::setIsRunning(false);
}


bool RobotController::solveTunnel(){
	console::debug("RC: Solve Tunnel Function");

	navControl->moveUntil(700, MOVEMENT::FORWARD, SPEED::HALF );
	//storageControl->setDrivePostion();


	return false;
}


bool RobotController::solveZoneA(){

	return false;
}
bool RobotController::solveZoneB(){

	return false;
}
bool RobotController::solveZoneC(){

	return false;
}



bool RobotController::scanRail(){

	return false;
}

bool RobotController::dance(){

	//return to start....

	return false;
}





//
//
//// Navigate through tunnel
////determine which port
//PortConfig pc = navControl->determinePort();
//
//
//if(pc == PortConfig::A){
//	//solve port A (right out of tunnel)
//	console::debug("Current Port Config: A");
//
//}
//if(pc == PortConfig::B){
//	//solve port B (left out of tunnel)
//	console::debug("Current Port Config: B");
//
//
//}
//
//
//
//navControl->moveUntil(300, MOVEMENT::FORWARD);
//storageControl->setDrivePostion();
//navControl->turn(0,0);
//
//
//
//
//while(0){
//	navControl->moveUntil(300, MOVEMENT::FORWARD);
//	navControl->turn(0,0);
//}
//
//
//
//if(pc == PortConfig::A){
//	//turn right
//	console::debug("Current Port Config: A");
//	//navControl->move(WALL_FOLLOWING::LEFT, 300, 0,200);
//}
//if(pc == PortConfig::B){
//	//turn left
//	console::debug("Current Port Config: B");
//	//navControl->move(WALL_FOLLOWING::RIGHT, 300, 0,200);
//}
//
////armControl->startup();
