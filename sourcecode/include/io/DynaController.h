/*
 * DynaController.h
 *
 *  Created on: Mar 21, 2016
 *      Author: Tobie
 */

#ifndef HEADERS_SERVOS_DYNACONTROLLER_H_
#define HEADERS_SERVOS_DYNACONTROLLER_H_
#include "DynaServo.h"
#include <termio.h>
#include "dynamixel.h"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>

using namespace std;

#define DEFAULT_DEVICE_ID 0
#define DEFAULT_BAUDRATE 1000000L
#define MODEL_ADDR 0

namespace dyn {

enum SERVO_TYPE {
	NONE = 0x0000,
	UNKNOWN = 0x0001,
	AX_12W = 0x012C,
	AX_12PA = 0x000C,
	AX_18FA = 0x0012,
	MX_12W = 0x0168,
	MX_28ATT = 0x001D,
	MX_64ATT = 0x0136,
	MX_106T = 0x0140
};

class DynaController {
public:
	DynaController(int deviceID = DEFAULT_DEVICE_ID, long baud = DEFAULT_BAUDRATE);
	virtual ~DynaController();
	bool connect();
	void disconnect();
	bool isConnected();
	bool hasServo(int id);
	SERVO_TYPE getServoType(int id);
	DynaServo getServo(int id) throw (string);
	string getServoTypeString(SERVO_TYPE tp);

private:
	int devID;
	long baudRate;

};

} /* namespace dyn */

#endif /* HEADERS_SERVOS_DYNACONTROLLER_H_ */
