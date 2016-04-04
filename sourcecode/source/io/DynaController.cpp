/*
 * DynaController.cpp
 *
 *  Created on: Mar 21, 2016
 *      Author: Tobie
 */

#include "../../include/io/DynaController.h"

namespace dyn {

DynaController::DynaController(int deviceID, long baud) {
	devID = deviceID;
	baudRate = baud;
}

DynaController::~DynaController() {
	disconnect();
}

bool DynaController::connect() {
	if (dxl_connected() == 1) {
		return true;
	}
	uint8_t baudnum = ((2000000 / baudRate) - 1) & 0xFF;
	if (dxl_initialize(devID, baudnum) == 0) {
		return false;
	} else {
		return true;
	}
}

void DynaController::disconnect() {
	dxl_terminate();
}

bool DynaController::isConnected() {
	if (dxl_connected() == 1) {
		return true;
	} else {
		return false;
	}
}

bool DynaController::hasServo(int id) {
	if (isConnected()) {
		dxl_ping(id);
		int CommStatus = dxl_get_result();
		if (CommStatus == COMM_RXSUCCESS) {
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

SERVO_TYPE DynaController::getServoType(int id) {
	int data = dxl_read_word(id, MODEL_ADDR);
	int CommStatus = dxl_get_result();
	if (CommStatus == COMM_RXSUCCESS) {
		switch (data) {
		case ((int) AX_12W):
			return AX_12W;
		case ((int) AX_12PA):
			return AX_12PA;
		case ((int) AX_18FA):
			return AX_18FA;
		case ((int) MX_12W):
			return MX_12W;
		case ((int) MX_28ATT):
			return MX_28ATT;
		case ((int) MX_64ATT):
			return MX_64ATT;
		case ((int) MX_106T):
			return MX_106T;
		default:
			return UNKNOWN;
		}
	} else {
		return NONE;
	}
}

DynaServo DynaController::getServo(int id) throw (string){
	if(!isConnected()){
		throw string("Error: Dynamixel Controller not connected.");
	}
	DynaServo s(id, baudRate);
	return s;
}

string DynaController::getServoTypeString(dyn::SERVO_TYPE tp) {
	switch (tp) {
	case (AX_12W):
		return string("AX_12W");
	case (AX_12PA):
		return string("AX_12PA");
	case (AX_18FA):
		return string("AX_18FA");
	case (MX_12W):
		return string("MX_12W");
	case (MX_28ATT):
		return string("MX_28ATT");
	case (MX_64ATT):
		return string("MX_64ATT");
	case (MX_106T):
		return string("MX_106T");
	case (NONE):
			return string("NONE");
	case (UNKNOWN):
			return string("UNKNOWN");
	default:
		return "";
	}
}

} /* namespace dyn */
