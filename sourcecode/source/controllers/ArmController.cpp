#include <iostream>

#include "../../include/system/console.h"
#include "../../include/controllers/ArmController.h"
#include "../../include/globals.h"
#include "../../include/io/StepperMotor.h"
#include "../../include/controllers/dynamixel.h"
#include "../../include/io/DynaMX12WServo.h"

using namespace std;
using namespace blaze;

#define P_GOAL_POSITION_L	30
#define P_GOAL_POSITION_H	31
#define P_PRESENT_POSITION_L	36
#define P_PRESENT_POSITION_H	37
#define P_MOVING		46

ArmController::ArmController(StepperMotor *sc) throw (string) {
	stepperLocation = -1;
	stepper = sc;
	isMove = false;
	//FIXME: implment code in controller to actually use moveComplete
	moveComplete = true;
	//Stop stepper motors
	stepper->disable();
	stepper->sleep();
	//setup arm constraints
	constriants = new ArmConstraint();
	//setup base servo constraints
	ArmConstraint::ServoCon baseSC;
	baseSC.id = BASE_SERVO_ID;
	baseSC.maxAngle = BASE_MAX_ANG;
	baseSC.minAngle = BASE_MIN_ANG;
	constriants->setServoConstraint(baseSC);
	//setup link1 servo constraints
	ArmConstraint::ServoCon link1SC;
	link1SC.id = LINK_1_SERVO_ID;
	link1SC.maxAngle = LINK_1_MAX_ANG;
	link1SC.minAngle = LINK_1_MIN_ANG;
	constriants->setServoConstraint(link1SC);
	//setup link2 servo constraints
	ArmConstraint::ServoCon link2SC;
	link2SC.id = LINK_2_SERVO_ID;
	link2SC.maxAngle = LINK_2_MAX_ANG;
	link2SC.minAngle = LINK_2_MIN_ANG;
	constriants->setServoConstraint(link2SC);
	//setup link3 servo constraints
	ArmConstraint::ServoCon link3SC;
	link3SC.id = LINK_3_SERVO_ID;
	link3SC.maxAngle = LINK_3_MAX_ANG;
	link3SC.minAngle = LINK_3_MIN_ANG;
	constriants->setServoConstraint(link3SC);
	//setup rotate servo constraints
	ArmConstraint::ServoCon rotateSC;
	rotateSC.id = ROTATE_WRIST_SERVO_ID;
	rotateSC.maxAngle = ROTATE_MAX_ANG;
	rotateSC.minAngle = ROTATE_MIN_ANG;
	constriants->setServoConstraint(rotateSC);
	//setup gripper servo constraints
	ArmConstraint::ServoCon gripperSC;
	gripperSC.id = GRIPPER_SERVO_ID;
	gripperSC.maxAngle = GRIPPER_MAX_ANG;
	gripperSC.minAngle = GRIPPER_MIN_ANG;
	constriants->setServoConstraint(gripperSC);
	//get arm servo's
	controller = new DynaController();
	if (!controller->connect()) {
		throw string("Unable to connect to AX2USB dongle.");
	}
	static DynaServo b = controller->getServo(BASE_SERVO_ID);
	base = &b;
	static DynaServo lk1 = controller->getServo(LINK_1_SERVO_ID);
	link1 = &lk1;
	static DynaServo lk2 = controller->getServo(LINK_2_SERVO_ID);
	link2 = &lk2;
	static DynaServo lk3 = controller->getServo(LINK_3_SERVO_ID);
	link3 = &lk3;
	static DynaServo rt = controller->getServo(ROTATE_WRIST_SERVO_ID);
	rotate = &rt;
	static DynaServo gr = controller->getServo(GRIPPER_SERVO_ID);
	gripper = &gr;
	/////////////////////Turn on Servo LED///////////////////////
	base->setLEDOn(true);
	link1->setLEDOn(true);
	link2->setLEDOn(true);
	link3->setLEDOn(true);
	rotate->setLEDOn(true);
	gripper->setLEDOn(true);
	////////////////////////////////////////////////////////////
	/////////////////////Setup alarm conditions/////////////////
	DynaServo::alarm alarm;
	alarm.angleLimitError = true;
	alarm.inputVoltageError = true;
	alarm.overHeatingError = true;
	alarm.overloadError = true;
	alarm.rangeError = true;
	base->setAlamrShutdownConditions(alarm);
	link1->setAlamrShutdownConditions(alarm);
	link2->setAlamrShutdownConditions(alarm);
	link3->setAlamrShutdownConditions(alarm);
	rotate->setAlamrShutdownConditions(alarm);
	gripper->setAlamrShutdownConditions(alarm);
	base->setAlarmLedConditions(alarm);
	link1->setAlarmLedConditions(alarm);
	link2->setAlarmLedConditions(alarm);
	link3->setAlarmLedConditions(alarm);
	rotate->setAlarmLedConditions(alarm);
	gripper->setAlarmLedConditions(alarm);
	////////////////////////////////////////////////////////////
	/////////////////////Set arm servo speed////////////////////
	base->setSpeed(BASE_SPEED);
	link1->setSpeed(LINK_1_SPEED);
	link2->setSpeed(LINK_2_SPEED);
	link3->setSpeed(LINK_3_SPEED);
	rotate->setSpeed(ROTATE_SPEED);
	gripper->setSpeed(GRIPPER_SPEED);
	////////////////////////////////////////////////////////////

}

ArmController::~ArmController() {
	base->~DynaServo();
	link1->~DynaServo();
	link2->~DynaServo();
	link3->~DynaServo();
	rotate->~DynaServo();
	gripper->~DynaServo();
	controller->~DynaController();
	constriants->~ArmConstraint();
	stepper->~StepperMotor();
}

void ArmController::init() {
	//setup Arm/Stepper/Switches

	//Stepper Initialization
	//this->stepper = StepperMotor(gpio_stepper_ms1,gpio_stepper_ms2,gpio_stepper_ms3,gpio_stepper_step,gpio_stepper_sleep, gpio_stepper_dir, gpio_stepper_en, gpio_stepper_lsw, gpio_stepper_rsw, 60, 200);
}

void ArmController::startup() {
//	this->isMoving = true;
//	console::debug("Arm Start move....");
//
//		//this->servo23 = DynaMX12WServo(1,23,115200);
//		this->servo20.connect();
//
///*
//		this->servo23.setSpeed(50);
//		this->servo23.setServoPosition(815);
//		this->servo22.setSpeed(50);
//	*/
//
//		console::debug("Servo 20 MaxAngle: " + to_string(this->servo20.getPosition()));
//		console::debug("Servo 21 MaxAngle: " + to_string(this->servo21.getPosition()));
//		console::debug("Servo 22 MaxAngle: " + to_string(this->servo22.getPosition()));
//		console::debug("Servo 23 MaxAngle: " + to_string(this->servo23.getPosition()));
//		console::debug("Servo 24 MaxAngle: " + to_string(this->servo24.getPosition()));
//		console::debug("Servo 25 MaxAngle: " + to_string(this->servo25.getPosition()));
//
//		console::debug("Servo 20 Temp: " + to_string(this->servo20.getTemperatureF()));
//		console::debug("Servo 21 Temp: " + to_string(this->servo21.getTemperatureF()));
//		console::debug("Servo 22 Temp: " + to_string(this->servo22.getTemperatureF()));
//		console::debug("Servo 23 Temp: " + to_string(this->servo23.getTemperatureF()));
//		console::debug("Servo 24 Temp: " + to_string(this->servo24.getTemperatureF()));
//		console::debug("Servo 25 Temp: " + to_string(this->servo25.getTemperatureF()));
//
//
//		this->servo23.closeConnection();
//
//		this->isMoving = false;
//

}
/*
 void ArmController::setArmPos(int s21, int s22, int s23, s24){




 console::debug("Servo 20: " + this->servo20.getTemperatureF());
 console::debug("Servo 21: " + this->servo21.getTemperatureF());
 console::debug("Servo 22: " + this->servo22.getTemperatureF());
 console::debug("Servo 23: " + this->servo23.getTemperatureF());
 console::debug("Servo 24: " + this->servo24.getTemperatureF());
 console::debug("Servo 25: " + this->servo25.getTemperatureF());

 }*/

void ArmController::stopNow() {

	//Stop stepper motors
	stepper->disable();
	stepper->sleep();
}

void ArmController::moveStepper(int mm) {

//	stepper->setDirection(StepperMotor::CLOCKWISE);
	stepper->enable();
	stepper->wake();
	usleep(2000); //wait for driver to wake up
	stepperLocation += mm;
	int step = 10*mm;
	stepper->setStepMode(StepperMotor::STEP_MODE::STEP_HALF);
	stepper->step(step);

}

void PrintCommStatus(int CommStatus) {
	switch (CommStatus) {
	case COMM_TXFAIL:
		printf("COMM_TXFAIL: Failed transmit instruction packet!\n");
		break;

	case COMM_TXERROR:
		printf("COMM_TXERROR: Incorrect instruction packet!\n");
		break;

	case COMM_RXFAIL:
		printf("COMM_RXFAIL: Failed get status packet from device!\n");
		break;

	case COMM_RXWAITING:
		printf("COMM_RXWAITING: Now recieving status packet!\n");
		break;

	case COMM_RXTIMEOUT:
		printf("COMM_RXTIMEOUT: There is no status packet!\n");
		break;

	case COMM_RXCORRUPT:
		printf("COMM_RXCORRUPT: Incorrect status packet!\n");
		break;

	default:
		printf("This is unknown error code!\n");
		break;
	}
}

double ArmController::getBaseAngle() {
	return base->getAngle();
}

void ArmController::setBaseAngle(double angle) throw (string) {
	if (!constriants->checkAngle(base, angle)) {
		ostringstream s1;
		s1 << "Invalid base angle: " << angle;
		throw s1.str();
	}
	base->setAngle(angle);
}

double ArmController::getLink1Angle() {
	return link1->getAngle();
}

void ArmController::setLink1Angle(double angle) throw (string) {
	if (!constriants->checkAngle(link1, angle)) {
		ostringstream s1;
		s1 << "Invalid link1 angle: " << angle;
		throw s1.str();
	}
	link1->setAngle(angle);
}

double ArmController::getLink2Angle() {
	return link2->getAngle();
}

void ArmController::setLink2Angle(double angle) throw (string) {
	if (!constriants->checkAngle(link2, angle)) {
		ostringstream s1;
		s1 << "Invalid link2 angle: " << angle;
		throw s1.str();
	}
	link2->setAngle(angle);
}

double ArmController::getLink3Angle() {
	return link3->getAngle();
}

void ArmController::setLink3Angle(double angle) throw (string) {
	if (!constriants->checkAngle(link3, angle)) {
		ostringstream s1;
		s1 << "Invalid link3 angle: " << angle;
		throw s1.str();
	}
	link3->setAngle(angle);
}

double ArmController::getRotateAngle() {
	return rotate->getAngle();
}

void ArmController::setRotateAngle(double angle) throw (string) {
	if (!constriants->checkAngle(rotate, angle)) {
		ostringstream s1;
		s1 << "Invalid rotate angle: " << angle;
		throw s1.str();
	}
	rotate->setAngle(angle);
}

double ArmController::getGripperAngle() {
	return gripper->getAngle();
}

void ArmController::setGripperAngle(double angle) throw (string) {
	if (!constriants->checkAngle(gripper, angle)) {
		ostringstream s1;
		s1 << "Invalid gripper angle: " << angle;
		throw s1.str();
	}
	gripper->setAngle(angle);
}

void ArmController::reset() {
	//FIXME: add code to set the arm to some default position.
}

ArmController::ServoStatus ArmController::getBaseStatus() {
	ServoStatus s;
	s.angle = base->getAngle();
	s.isMoving = base->isMoving();
	s.temperatureC = base->getTemperatureC();
	s.temperatureF = base->getTemperatureF();
	s.voltage = base->getVoltage();
	s.alarmCondition = base->getCurrentAlarms();
	return s;
}

ArmController::ServoStatus ArmController::getLink1Status() {
	ServoStatus s;
	s.angle = link1->getAngle();
	s.isMoving = link1->isMoving();
	s.temperatureC = link1->getTemperatureC();
	s.temperatureF = link1->getTemperatureF();
	s.voltage = link1->getVoltage();
	s.alarmCondition = link1->getCurrentAlarms();
	return s;
}

ArmController::ServoStatus ArmController::getLink2Status() {
	ServoStatus s;
	s.angle = link2->getAngle();
	s.isMoving = link2->isMoving();
	s.temperatureC = link2->getTemperatureC();
	s.temperatureF = link2->getTemperatureF();
	s.voltage = link2->getVoltage();
	s.alarmCondition = link2->getCurrentAlarms();
	return s;
}

ArmController::ServoStatus ArmController::getLink3Status() {
	ServoStatus s;
	s.angle = link3->getAngle();
	s.isMoving = link3->isMoving();
	s.temperatureC = link3->getTemperatureC();
	s.temperatureF = link3->getTemperatureF();
	s.voltage = link3->getVoltage();
	s.alarmCondition = link3->getCurrentAlarms();
	return s;
}

ArmController::ServoStatus ArmController::getRotateStatus() {
	ServoStatus s;
	s.angle = rotate->getAngle();
	s.isMoving = rotate->isMoving();
	s.temperatureC = rotate->getTemperatureC();
	s.temperatureF = rotate->getTemperatureF();
	s.voltage = rotate->getVoltage();
	s.alarmCondition = rotate->getCurrentAlarms();
	return s;
}

ArmController::ServoStatus ArmController::getGripperStatus() {
	ServoStatus s;
	s.angle = gripper->getAngle();
	s.isMoving = gripper->isMoving();
	s.temperatureC = gripper->getTemperatureC();
	s.temperatureF = gripper->getTemperatureF();
	s.voltage = gripper->getVoltage();
	s.alarmCondition = gripper->getCurrentAlarms();
	return s;
}

void ArmController::zoneATopBlock() {
	//base 180
	//lk1 191
	//lk2 120
	//lk3 86
	//rt 90
	moveToLocation(180.0, 191.0, 120.0, 86.0, 90.0);
}

void ArmController::zoneABottomBlock() {
	//base 180
	//lk1 214
	//lk2 109
	//lk3 81
	//rt 90
	moveToLocation(180.0, 214.0, 109.0, 81.0, 90.0);
}

void ArmController::zoneAReset() {
	//base 180
	//lk1 239
	//lk2 128
	//lk3 47
	//rt 90
	moveToLocation(180.0, 239.0, 128.0, 47.0, 90.0);
}

void ArmController::zoneBTopBlockLong() {
	//base 360
	//lk1 221
	//lk2 253
	//lk3 154
	//rt 90
	moveToLocation(360.0, 221.0, 253.0, 154.0, 90.0);
}

void ArmController::zoneBBottomBlockLong() {
	//base 360
	//lk1 235
	//lk2 253
	//lk3 147
	//rt 90
	moveToLocation(360.0, 235.0, 253.0, 147.0, 90.0);
}

void ArmController::zoneBTopBlockShortFront() {
	//base 360
	//lk1 221
	//lk2 253
	//lk3 154
	//rt 90
	moveToLocation(360.0, 221.0, 253.0, 154.0, 90.0);
}

void ArmController::zoneBBottomBlockShortFront() {
	//base 360
	//lk1 235
	//lk2 253
	//lk3 147
	//rt 90
	moveToLocation(360.0, 235.0, 253.0, 147.0, 90.0);
}

void ArmController::zoneBTopBlockShortBack() {
	//base 180
	//lk1 170
	//lk2 176
	//lk3 95
	//rt 270
	moveToLocation(180.0, 170.0, 176.0, 95.0, 270.0);
}

void ArmController::zoneBBottomBlockShortBack() {
	//base 180
	//lk1 230
	//lk2 86
	//lk3 133
	//rt 270
	moveToLocation(180.0, 230.0, 86.0, 133.0, 270.0);
}

void ArmController::zoneBReset() {
	//base 360
	//lk1 150
	//lk2 236
	//lk3 236
	//rt 90
	moveToLocation(360.0, 150.0, 236.0, 236.0, 90.0);
}

void ArmController::zoneBResetShort() {
	//base 180
	//lk1 239
	//lk2 152
	//lk3 47
	//rt 270
	moveToLocation(180.0, 239.0, 152.0, 47.0, 270.0);
}

void ArmController::zoneCTopBlock() {
	moveToLocation(360.0, 210.0, 227.0, 164.0, 90.0);
	usleep(400000);
	//base 360
	//lk1 231
	//lk2 253
	//lk3 164
	//rt 90
	moveToLocation(360.0, 231.0, 253.0, 164.0, 90.0);
}

void ArmController::zoneCBottomBlock() {
	moveToLocation(360.0, 210.0, 227.0, 164.0, 90.0);
	usleep(400000);
	//base 360
	//lk1 239
	//lk2 25
	//lk3 164
	//rt 90
	moveToLocation(360.0, 239.0, 253.0, 164.0, 90.0);
}

void ArmController::zoneCReset() {
	moveToLocation(360.0, 197.0, 227.0, 238.0, 90.0);
}

double ArmController::calculateStepSize(double loc, double dst){
	double diff = abs((loc - dst));
	double step = diff;
	if((diff > 100.0) && (diff < 50.0)){
		step = diff/SERVO_STEP_DIVIDER_HIGH;
	} else if((diff >= 50.0) && (diff < 20.0)){
		step = diff/SERVO_STEP_DIVIDER_LOW;
	}
	return step;
}

double ArmController::angleStep(double loc, double dst) {
	double diff = angleDiff(loc, dst);

	double absDiff = abs(diff);
//	double step = calculateStepSize(loc, dst);
//	double move = 0.0;
//	if (angleEqaul(absDiff, 0.0, ANGLE_PRESISION)) {
//		move = 0.0;
//	} else if (angleGreater(absDiff, step, ANGLE_PRESISION)) {
//		move = (diff > 0.0) ? (-1 * step) : step;
//	} else if (angleLess(absDiff, step, ANGLE_PRESISION)) {
//		move = (diff > 0.0) ? (-1 * absDiff) : absDiff;
//	}
	return (diff > 0.0) ? (-1 * absDiff) : absDiff;//move;
}

double ArmController::angleDiff(double loc, double dst) {
	return loc - dst;
}

bool ArmController::angleEqaul(double a, double b, double pres) {
	return abs((a - b)) < abs(pres);
}

bool ArmController::angleGreater(double a, double b, double pres) {
	double diff = (a - b);
	if (abs(diff) < abs(pres)) {
		return false;
	} else if (diff < 0.0) {
		return false;
	} else {
		return true;
	}
}

bool ArmController::angleLess(double a, double b, double pres) {
	double diff = (b - a);
	if (abs(diff) < abs(pres)) {
		return false;
	} else if (diff < 0.0) {
		return false;
	} else {
		return true;
	}
}

bool ArmController::moveServo(DynaServo *s, double step) {
	double angle = s->getAngle();
	double next = angle + step;
	if (!constriants->checkAngle(s, next)) {
		ostringstream s1;
		s1 << "Invalid servo angle<" << s->getID() << ">: " << next;
		console::error(s1.str());
		if (constriants->isMaxAngle(s, next)) {
			s->setAngle(constriants->getServoConstraint(s->getID()).maxAngle);
		}
		if (constriants->isMinAngle(s, next)) {
			s->setAngle(constriants->getServoConstraint(s->getID()).minAngle);
		}
		return false;
	}
	s->setAngle(next);
	return true;
}

bool ArmController::servoReachedLocation(DynaServo *s, double dst, double pres){
	if (!constriants->checkAngle(s, dst)){
		if (constriants->isMaxAngle(s, dst)) {
			return true;
		}
		if (constriants->isMinAngle(s, dst)) {
			return true;
		}
	}
	return(angleEqaul(s->getAngle(), dst, pres));
}

bool ArmController::moveToLocation(double bs, double lk1, double lk2,
		double lk3, double rt) {
	bool reachedLocation = false;
	isMove = true;
	while (!reachedLocation) {
		reachedLocation = true;
		moveServo(base, angleStep(base->getAngle(), bs));
		reachedLocation &= servoReachedLocation(base, bs, ARM_SERVO_ANGLE_DST_PRES);
		moveServo(link1, angleStep(link1->getAngle(), lk1));
		reachedLocation &= servoReachedLocation(link1, lk1, ARM_SERVO_ANGLE_DST_PRES);
		moveServo(link2, angleStep(link2->getAngle(), lk2));
		reachedLocation &= servoReachedLocation(link2, lk2, ARM_SERVO_ANGLE_DST_PRES);
		moveServo(link3, angleStep(link3->getAngle(), lk3));
		reachedLocation &= servoReachedLocation(link3, lk3, ARM_SERVO_ANGLE_DST_PRES);
		moveServo(rotate, angleStep(rotate->getAngle(), rt));
		reachedLocation &= servoReachedLocation(rotate, rt, ARM_SERVO_ANGLE_DST_PRES);
//		usleep(100000);
	}
	isMove = false;
	return true;
}

bool blaze::ArmController::isConnected() {
	return controller->isConnected();
}

bool blaze::ArmController::isMoving() {
	return isMove;
}

bool blaze::ArmController::moveCompleted() {
	return moveComplete;
}

void blaze::ArmController::homeStepper() {
	stepper->goHome();
}

bool blaze::ArmController::isStepperHome() {
	return stepper->isHome();
}

int blaze::ArmController::getStepperDistance() {
	return stepperLocation;
}

bool blaze::ArmController::isStepperAtLimit() {
	return stepper->isAtLimit();
}
