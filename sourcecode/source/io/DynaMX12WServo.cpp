/*
 * Dynamixel.cpp
 *
 *  Created on: Mar 16, 2016
 *      Author: Tobie
 */
#include <unistd.h>
#include "../../include/globals.h"
#include "../../include/system/console.h"
#include "../../include/io/DynaMX12WServo.h"


//Define Address Data Table in servo's
#define MODEL_ADDR   	0
#define VERSION_ADDR 	2
#define ID_NUM_ADDR  	3
#define BAUD_ADDR   	4
#define TRN_DEL_ADDR    5
#define CW_ANG_LIM_ADDR 6
#define CCW_ANG_LIM_ADDR 8
#define TMP_LIMIT_ADDR  11
#define LOW_VOLT_LIM_ADDR 12
#define HIGH_VOLT_LIM_ADDR 13
#define MAX_TORQUE_ADDR 14
#define STATES_RTN_ADDR 16
#define ALARM_LED_ADDR 17
#define ALARM_SHUT_ADDR 18
#define MULTI_TURN_OFFSET 20
#define RESOLUTION_DIVIDER 22
#define TOR_ENB_ADDR 24
#define LED_ON_OFF_ADDR 25

#define D_GAIN 26
#define I_GAIN 27
#define P_GAIN 28

#define CCW_SLOPE_MAR_ADDR 29
#define GOAL_POS_ADDR 30
#define MOV_SPEED_ADDR 32
#define TOR_LIMIT_ADDR 34
#define PRES_POS_ADDR 36
#define PRES_SPEED_ADDR 38
#define PRES_LOAD_ADDR 40
#define PRES_VOLT_ADDR 42
#define PRES_TEMP_ADDR 43
#define REG_INST_ADDR 44
#define MOVING_ADDR 46
#define LOCK_EEPROM_ADDR 47
#define PUNCH_ADDR 48
#define ANGLE_UNIT (300.0/1023.0)
#define GOAL_ACCELERATION 73

//Global functions
#define GLOBAL_ADDR 254
DynaMX12WServo::DynaMX12WServo(){};

DynaMX12WServo::DynaMX12WServo(int devID, int serID, long baud) {
	deviceID = devID;
	servoID = serID;
	baudRate = baud;
	connected = false;
	err = NONE;
	singleMove = false;
	nextAng = 0;
}

DynaMX12WServo::DynaMX12WServo(const DynaMX12WServo& orig) {
	deviceID = orig.deviceID;
	servoID = orig.servoID;
	baudRate = orig.baudRate;
	connected = orig.connected;
	err = orig.err;
	singleMove = orig.singleMove;
	nextAng = orig.nextAng;
}

DynaMX12WServo::~DynaMX12WServo() {
	dxl_terminate();
}

ERROR_CODE DynaMX12WServo::getErrorCode() {
	return err;
}

double DynaMX12WServo::getVoltage() {
	int data = dxl_read_byte(servoID, PRES_VOLT_ADDR);
	int CommStatus = dxl_get_result();
	if (CommStatus == COMM_RXSUCCESS) {
		return ((double) data) / 10.0;
	} else {
		setCommunicationError(CommStatus);
		return -1.0;
	}
}

double DynaMX12WServo::getAngle() {
	int data = dxl_read_word(servoID, PRES_POS_ADDR);
	int CommStatus = dxl_get_result();
	if (CommStatus == COMM_RXSUCCESS) {
		return ((double) data) * ANGLE_UNIT;
	} else {
		setCommunicationError(CommStatus);
		return -1.0;
	}
}

double DynaMX12WServo::getSpeed() {
	int data = dxl_read_word(servoID, MOV_SPEED_ADDR);
	int CommStatus = dxl_get_result();
	if (CommStatus == COMM_RXSUCCESS) {
		if (data < 1024) {
			return ((double) data) * 0.111; //going CCW
		} else {
			return ((double) (data - 1024)) * -0.111; //going CW
		}
	} else {
		setCommunicationError(CommStatus);
		return 0.0;
	}
}

void DynaMX12WServo::setAngle(double angle) {
	if (singleMove) {
		nextAng = angle;
		return;
	} else {
		nextAng = angle;
		setServoAngle(angle);
	}
}

void DynaMX12WServo::setSpeed(double rpm) {
	int data = (rpm == 0.0) ? 0 : ROUND_2_INT((rpm / 0.111));
	if (data < 0) {
		data = 1;
	}
	if (data > 1023) {
		data = 1023;
	}
	dxl_write_word(servoID, MOV_SPEED_ADDR, data);
}

int DynaMX12WServo::getModelNumber() {
	int data = dxl_read_word(servoID, MODEL_ADDR);
	int CommStatus = dxl_get_result();
	if (CommStatus == COMM_RXSUCCESS) {
		return data;
	} else {
		setCommunicationError(CommStatus);
		return -1;
	}
}

int DynaMX12WServo::getFirmwareVers() {
	int data = dxl_read_byte(servoID, VERSION_ADDR);
	int CommStatus = dxl_get_result();
	if (CommStatus == COMM_RXSUCCESS) {
		return data;
	} else {
		setCommunicationError(CommStatus);
		return -1;
	}
}

long DynaMX12WServo::getBaudrate() {
	int data = dxl_read_byte(servoID, BAUD_ADDR);
	int CommStatus = dxl_get_result();
	if (CommStatus == COMM_RXSUCCESS) {
		return 2000000 / (((int) data) + 1);
	} else {
		setCommunicationError(CommStatus);
		return -1;
	}
}

void DynaMX12WServo::setBaudrate(long bps) {
	if (bps > 2000000) {
		bps = 2000000;
	}
	if (bps < 7484) {
		bps = 7484;
	}
	baudRate = bps;
	int data = ((2000000 / bps) - 1) & 0xFF;
	dxl_write_byte(servoID, BAUD_ADDR, data);
}

int DynaMX12WServo::getTransDelay() {
	int data = dxl_read_byte(servoID, TRN_DEL_ADDR);
	int CommStatus = dxl_get_result();
	if (CommStatus == COMM_RXSUCCESS) {
		return ((int) data) * 2;
	} else {
		setCommunicationError(CommStatus);
		return -1;
	}
}

void DynaMX12WServo::setTransDelay(int usec) {
	if (usec > 508) {
		usec = 508;
	}
	if (usec < 0) {
		usec = 0;
	}
	int data = (usec == 0) ? 0 : usec / 2;
	dxl_write_byte(servoID, TRN_DEL_ADDR, data);
}

int DynaMX12WServo::getID() {
	return servoID;
}

void DynaMX12WServo::setID(int id) {
	if (id > 252) {
		err = ID;
		return;
	}
	if (id < 0) {
		err = ID;
		return;
	}
	bool inuses = false;
	int data = dxl_read_byte(ID, ID_NUM_ADDR);
	int CommStatus = dxl_get_result();
	if (CommStatus == COMM_RXSUCCESS) {
		inuses = (ID == data);
	}
	if (!inuses) {
		dxl_write_byte(servoID, ID_NUM_ADDR, id);
		servoID = id;
	} else {
		err = ID;
	}

}

bool DynaMX12WServo::connect() {
	uint8_t baudnum = ((2000000 / baudRate) - 1) & 0xFF;
	if (dxl_initialize(deviceID, baudnum) == 0) {
		err = CONNECTION;
		connected = false;
	} else {
		connected = true;
	}
	return true;
}

bool DynaMX12WServo::isConnected() {
	return connected;
}

bool DynaMX12WServo::isMoving() {
	int data = dxl_read_byte(servoID, MOVING_ADDR);
	int CommStatus = dxl_get_result();
	if (CommStatus == COMM_RXSUCCESS) {
		return (data == 1);
	} else {
		setCommunicationError(CommStatus);
		return false;
	}
}

void DynaMX12WServo::closeConnection() {
	dxl_terminate();
}

void DynaMX12WServo::setSingleMove(bool move) {
	singleMove = move;
}

bool DynaMX12WServo::getSingleMove() {
	return singleMove;
}

bool DynaMX12WServo::moveServo() {
	if (!isValueEqual(nextAng, getAngle(), 1.0)) {
		double angle = getAngle();
		if (nextAng < angle) {
			if ((angle - ANGLE_INCREASE) < nextAng) {
				setServoAngle(nextAng);
				return true;
			}
			angle -= ANGLE_INCREASE;
			setServoAngle(angle);
			return false;
		} else if (nextAng > angle) {
			if ((angle + ANGLE_INCREASE) > nextAng) {
				setServoAngle(nextAng);
				return true;
			}
			angle += ANGLE_INCREASE;
			setServoAngle(angle);
			return false;
		} else {
			return true;
		}
	} else {
		return true;
	}
}

void DynaMX12WServo::setCommunicationError(int error) {
	switch (error) {
	case COMM_TXFAIL:
		err = TXFAIL;
		break;

	case COMM_TXERROR:
		err = TXFAIL;
		break;

	case COMM_RXFAIL:
		err = RXFAIL;
		break;

	case COMM_RXWAITING:
		err = RXWAITING;
		break;

	case COMM_RXTIMEOUT:
		err = RXTIMEOUT;
		break;

	case COMM_RXCORRUPT:
		err = RXCORRUPT;
		break;

	default:
		err = UNKNOWN_COMM;
		break;
	}
	err = CONNECTION;
}

double DynaMX12WServo::getRadians(void) {
	return ANG_TO_RAD(getAngle());
}

int DynaMX12WServo::getMaxAngle() {
	int data = dxl_read_word(servoID, CCW_ANG_LIM_ADDR);
	int CommStatus = dxl_get_result();
	if (CommStatus == COMM_RXSUCCESS) {
		return ((double) data) * ANGLE_UNIT;
	} else {
		setCommunicationError(CommStatus);
		return -1.0;
	}
}

double DynaMX12WServo::getTemperatureF() {
	int data = dxl_read_byte(servoID, PRES_TEMP_ADDR);
	int CommStatus = dxl_get_result();
	if (CommStatus == COMM_RXSUCCESS) {
		return (((double) data)*1.8)+32;
	} else {
		setCommunicationError(CommStatus);
		return -1.0;
	}
}

double DynaMX12WServo::getTemperatureC() {
	int data = dxl_read_byte(servoID, PRES_TEMP_ADDR);
		int CommStatus = dxl_get_result();
		if (CommStatus == COMM_RXSUCCESS) {
			return ((double) data);
		} else {
			setCommunicationError(CommStatus);
			return -1.0;
		}
}

void DynaMX12WServo::setServoAngle(double angle) {
	if (angle < 0.0) {
		angle = 0.0;
	}
	if (angle > 300.0) {
		angle = 300.0;
	}
	int data = (angle == 0.0) ? 0 : ROUND_2_INT((angle / ANGLE_UNIT));
	//cout << "AX12A angle: " << data << endl;
	dxl_write_word(servoID, GOAL_POS_ADDR, data);
	int moving = 1;
	while (moving == 1) {
		moving = dxl_read_byte(servoID, MOVING_ADDR);
		int CommStatus = dxl_get_result();
		if (CommStatus != COMM_RXSUCCESS) {
			break;
		}
		usleep(2000);
	}
}
void DynaMX12WServo::setServoPosition(double pos) {

	int data =  ROUND_2_INT(pos);
	dxl_write_word(servoID, GOAL_POS_ADDR, data);
	int moving = 1;
	while (moving == 1) {
		moving = dxl_read_byte(servoID, MOVING_ADDR);
		int CommStatus = dxl_get_result();
		if (CommStatus != COMM_RXSUCCESS) {
			console::debug("DynaMX12Servo: COMM RX SUCCESS: " + to_string(servoID));
			break;
		}
		usleep(2000);
	}
}

double DynaMX12WServo::getPosition() {
	int data = dxl_read_word(servoID, 36);
		int CommStatus = dxl_get_result();
		if (CommStatus == COMM_RXSUCCESS) {
			return ((double) data);
		} else {
			setCommunicationError(CommStatus);
			return -1.0;
		}
}
