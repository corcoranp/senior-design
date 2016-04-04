/*
 * Dynamixel.cpp
 *
 *  Created on: Mar 16, 2016
 *      Author: Tobie
 */

#include "../../include/io/DynaServo.h"

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
#define TOR_ENB_ADDR 24
#define LED_ON_OFF_ADDR 25
#define CW_COMP_MAR_ADDR 26
#define CCW_COMP_MAR_ADDR 27
#define CW_SLOPE_MAR_ADDR 28
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
//MX only regs
#define MULTI_TURN_OFFSET_ADDR 20
#define MULTI_TURN_RES_ADDR 22
#define D_GAIN_ADDR 26
#define I_GAIN_ADDR 27
#define P_GAIN_ADDR 28
#define GOAL_ACC_ADDR 73

//Predefined Values
#define AX_ANGLE_UNIT (300.0/1023.0)
#define MX_ANGLE_UNIT (360.0/4095.0)
#define AX_SPEED_UNIT (0.111)
#define MX_SPEED_UNIT (0.114)
#define AX_ANGLE_LIMIT (300.0)
#define MX_ANGLE_LIMIT (360.0)

//Global functions
#define GLOBAL_ADDR 254

DynaServo::DynaServo(int serID, long baud) {
	servoID = serID;
	baudRate = baud;
	connected = false;
	err = NO_ERROR;
	type = NONE;
	isAxServo = true;
	torqueShutdown = false;
	getModelNumber();
}

DynaServo::DynaServo(const DynaServo& orig) {
	servoID = orig.servoID;
	baudRate = orig.baudRate;
	connected = orig.connected;
	err = orig.err;
	type = orig.type;
	isAxServo = orig.isAxServo;
	torqueShutdown = orig.torqueShutdown;
}

DynaServo::~DynaServo() {
	setTorqueEnabled(false);
}

DynaServo::ERROR_CODE DynaServo::getErrorCode() {
	return err;
}

double DynaServo::getVoltage() {
	int data = dxl_read_byte(servoID, PRES_VOLT_ADDR);
	int CommStatus = dxl_get_result();
	if (CommStatus == COMM_RXSUCCESS) {
		return ((double) data) / 10.0;
	} else {
		setCommunicationError(CommStatus);
		return -1.0;
	}
}

double DynaServo::getAngle() {
	int data = dxl_read_word(servoID, PRES_POS_ADDR);
	int CommStatus = dxl_get_result();
	if (CommStatus == COMM_RXSUCCESS) {
		return ((double) data) * ((isAxServo) ? AX_ANGLE_UNIT : MX_ANGLE_UNIT);
	} else {
		setCommunicationError(CommStatus);
		return -1.0;
	}
}

double DynaServo::getSpeed() {
	int data = dxl_read_word(servoID, MOV_SPEED_ADDR);
	int CommStatus = dxl_get_result();
	if (CommStatus == COMM_RXSUCCESS) {
		if (data < 1024) {
			return ((double) data)
					* ((isAxServo) ? AX_SPEED_UNIT : MX_SPEED_UNIT); //going CCW
		} else {
			return ((double) (data - 1024))
					* (-1 * ((isAxServo) ? AX_SPEED_UNIT : MX_SPEED_UNIT)); //going CW
		}
	} else {
		setCommunicationError(CommStatus);
		return 0.0;
	}
}

void DynaServo::setAngle(double angle) {
	if (angle < 0.0) {
		angle = 0.0;
	}
	if (angle > ((isAxServo) ? AX_ANGLE_LIMIT : MX_ANGLE_LIMIT)) {
		angle = ((isAxServo) ? AX_ANGLE_LIMIT : MX_ANGLE_LIMIT);
	}
	if(!isTorqueEnabled()){
		alarm al = getCurrentAlarms();
		if(!al.overHeatingError && !al.inputVoltageError){
			setTorqueEnabled(true);
		}
	}
	int moving = dxl_read_byte(servoID, MOVING_ADDR);
	int CommStatus = dxl_get_result();
	if (CommStatus != COMM_RXSUCCESS) {
		setCommunicationError(CommStatus);
		return;
	}
	if (moving == 1) {
		return;
	}
	int data =
			(angle == 0.0) ?
					0 :
					ROUND_2_INT(
							(angle / ((isAxServo)?AX_ANGLE_UNIT:MX_ANGLE_UNIT)));
	cout << "Servo Angle: (" << angle << ((isAxServo)?"/300) -> (":"/360) -> (") << data << ((isAxServo)?"/1023)":"/4095)") << endl;
	dxl_write_word(servoID, GOAL_POS_ADDR, data);
	usleep(100);
}

void DynaServo::setSpeed(double rpm) {
	int data =
			(rpm == 0.0) ?
					0 :
					ROUND_2_INT(
							(rpm / ((isAxServo)?AX_SPEED_UNIT:MX_SPEED_UNIT)));
	cout << "Speed: " << rpm << " -> " << data << endl;
	if (data < 0) {
		data = 1;
	}
	if (data > 1023) {
		data = 1023;
	}
	dxl_write_word(servoID, MOV_SPEED_ADDR, data);
	usleep(100);
}

int DynaServo::getModelNumber() {
	int data = dxl_read_word(servoID, MODEL_ADDR);
	int CommStatus = dxl_get_result();
	if (CommStatus == COMM_RXSUCCESS) {
		switch (data) {
		case ((int) AX_12W):
			type = AX_12W;
			isAxServo = true;
			break;
		case ((int) AX_12PA):
			type = AX_12PA;
			isAxServo = true;
			break;
		case ((int) AX_18FA):
			type = AX_18FA;
			isAxServo = true;
			break;
		case ((int) MX_12W):
			type = MX_12W;
			isAxServo = false;
			break;
		case ((int) MX_28ATT):
			type = MX_28ATT;
			isAxServo = false;
			break;
		case ((int) MX_64ATT):
			type = MX_64ATT;
			isAxServo = false;
			break;
		case ((int) MX_106T):
			type = MX_106T;
			isAxServo = false;
			break;
		default:
			type = UNKNOWN;
			isAxServo = false;
		}
		return data;
	} else {
		setCommunicationError(CommStatus);
		return -1;
	}
}

int DynaServo::getFirmwareVers() {
	int data = dxl_read_byte(servoID, VERSION_ADDR);
	int CommStatus = dxl_get_result();
	if (CommStatus == COMM_RXSUCCESS) {
		return data;
	} else {
		setCommunicationError(CommStatus);
		return -1;
	}
}

long DynaServo::getBaudrate() {
	int data = dxl_read_byte(servoID, BAUD_ADDR);
	int CommStatus = dxl_get_result();
	if (CommStatus == COMM_RXSUCCESS) {
		return 2000000 / (((int) data) + 1);
	} else {
		setCommunicationError(CommStatus);
		return -1;
	}
}

void DynaServo::setBaudrate(long bps) {
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

int DynaServo::getTransDelay() {
	int data = dxl_read_byte(servoID, TRN_DEL_ADDR);
	int CommStatus = dxl_get_result();
	if (CommStatus == COMM_RXSUCCESS) {
		return ((int) data) * 2;
	} else {
		setCommunicationError(CommStatus);
		return -1;
	}
}

void DynaServo::setTransDelay(int usec) {
	if (usec > 508) {
		usec = 508;
	}
	if (usec < 0) {
		usec = 0;
	}
	int data = (usec == 0) ? 0 : usec / 2;
	dxl_write_byte(servoID, TRN_DEL_ADDR, data);
}

int DynaServo::getID() {
	return servoID;
}

void DynaServo::setID(int id) {
	if (id > 252) {
		err = ID;
		return;
	}
	if (id < 0) {
		err = ID;
		return;
	}
	bool inuses = false;
	int data = dxl_read_byte(id, ID_NUM_ADDR);
	int CommStatus = dxl_get_result();
	if (CommStatus == COMM_RXSUCCESS) {
		inuses = (id == data);
	}
	if (!inuses) {
		dxl_write_byte(servoID, ID_NUM_ADDR, id);
		servoID = id;
	} else {
		err = ID;
	}

}

bool DynaServo::isConnected() {
	return connected;
}

bool DynaServo::isMoving() {
	int data = dxl_read_byte(servoID, MOVING_ADDR);
	int CommStatus = dxl_get_result();
	if (CommStatus == COMM_RXSUCCESS) {
		return (data == 1);
	} else {
		setCommunicationError(CommStatus);
		return false;
	}
}

void DynaServo::setCommunicationError(int error) {
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

double DynaServo::getRadians(void) {
	return ANG_TO_RAD(getAngle());
}

int DynaServo::getMaxAngle() {
	int data = dxl_read_word(servoID, CCW_ANG_LIM_ADDR);
	int CommStatus = dxl_get_result();
	if (CommStatus == COMM_RXSUCCESS) {
		return ((double) data) * ((isAxServo) ? AX_ANGLE_UNIT : MX_ANGLE_UNIT);
	} else {
		setCommunicationError(CommStatus);
		return -1.0;
	}
}

double DynaServo::getTemperatureF() {
	int data = dxl_read_byte(servoID, PRES_TEMP_ADDR);
	int CommStatus = dxl_get_result();
	if (CommStatus == COMM_RXSUCCESS) {
		return (((double) data) * 1.8) + 32;
	} else {
		setCommunicationError(CommStatus);
		return -1.0;
	}
}

double DynaServo::getTemperatureC() {
	int data = dxl_read_byte(servoID, PRES_TEMP_ADDR);
	int CommStatus = dxl_get_result();
	if (CommStatus == COMM_RXSUCCESS) {
		return ((double) data);
	} else {
		setCommunicationError(CommStatus);
		return -1.0;
	}
}

double DynaServo::getCWAngleLimit() {
	int data = dxl_read_word(servoID, CW_ANG_LIM_ADDR);
	int CommStatus = dxl_get_result();
	if (CommStatus == COMM_RXSUCCESS) {
		return ((double) data) * ((isAxServo) ? AX_ANGLE_UNIT : MX_ANGLE_UNIT);
	} else {
		setCommunicationError(CommStatus);
		return -1.0;
	}
}

void DynaServo::setCWAngleLimit(double angle) {
	if (angle < 0.0) {
		angle = 0.0;
	}
	if (angle > ((isAxServo) ? AX_ANGLE_LIMIT : MX_ANGLE_LIMIT)) {
		angle = ((isAxServo) ? AX_ANGLE_LIMIT : MX_ANGLE_LIMIT);
	}
	int data =
			(angle == 0.0) ?
					0 :
					ROUND_2_INT(
							(angle / ((isAxServo)?AX_ANGLE_UNIT:MX_ANGLE_UNIT)));
	cout << "base: " << angle << " -> " << data << endl;
	dxl_write_word(servoID, CW_ANG_LIM_ADDR, data);
}

double DynaServo::getCCWAngleLimit() {
	int data = dxl_read_word(servoID, CCW_ANG_LIM_ADDR);
	int CommStatus = dxl_get_result();
	if (CommStatus == COMM_RXSUCCESS) {
		return ((double) data) * ((isAxServo) ? AX_ANGLE_UNIT : MX_ANGLE_UNIT);
	} else {
		setCommunicationError(CommStatus);
		return -1.0;
	}
}

void DynaServo::setCCWAngleLimit(double angle) {
	if (angle < 0.0) {
		angle = 0.0;
	}
	if (angle > ((isAxServo) ? AX_ANGLE_LIMIT : MX_ANGLE_LIMIT)) {
		angle = ((isAxServo) ? AX_ANGLE_LIMIT : MX_ANGLE_LIMIT);
	}
	int data =
			(angle == 0.0) ?
					0 :
					ROUND_2_INT(
							(angle / ((isAxServo)?AX_ANGLE_UNIT:MX_ANGLE_UNIT)));
	cout << "base: " << angle << " -> " << data << endl;
	dxl_write_word(servoID, CCW_ANG_LIM_ADDR, data);
}

double DynaServo::getTemperatureLimitF() {
	int data = dxl_read_byte(servoID, TMP_LIMIT_ADDR);
	int CommStatus = dxl_get_result();
	if (CommStatus == COMM_RXSUCCESS) {
		return (((double) data) * 1.8) + 32;
	} else {
		setCommunicationError(CommStatus);
		return -1.0;
	}
}

double DynaServo::getTemperatureLimitC() {
	int data = dxl_read_byte(servoID, TMP_LIMIT_ADDR);
	int CommStatus = dxl_get_result();
	if (CommStatus == COMM_RXSUCCESS) {
		return ((double) data);
	} else {
		setCommunicationError(CommStatus);
		return -1.0;
	}
}

double DynaServo::getLowVoltageAlarmValue() {
	int data = dxl_read_byte(servoID, LOW_VOLT_LIM_ADDR);
	int CommStatus = dxl_get_result();
	if (CommStatus == COMM_RXSUCCESS) {
		return ((double) data) / 10.0;
	} else {
		setCommunicationError(CommStatus);
		return -1.0;
	}
}

double DynaServo::getHighVoltageAlarmValue() {
	int data = dxl_read_byte(servoID, HIGH_VOLT_LIM_ADDR);
	int CommStatus = dxl_get_result();
	if (CommStatus == COMM_RXSUCCESS) {
		return ((double) data) / 10.0;
	} else {
		setCommunicationError(CommStatus);
		return -1.0;
	}
}

double DynaServo::getMaxTorqueLimit() {
	int data = dxl_read_word(servoID, MAX_TORQUE_ADDR);
	int CommStatus = dxl_get_result();
	if (CommStatus == COMM_RXSUCCESS) {
		return ((double) data) / 1023.0;
	} else {
		setCommunicationError(CommStatus);
		return 0.0;
	}
}

void DynaServo::setMaxTorqueLimit(double maxTorque) {
	if (maxTorque < 0.0) {
		maxTorque = 0.0;
	}
	if (maxTorque > 100.0) {
		maxTorque = 100.0;
	}
	int data = (maxTorque == 0.0) ? 0 : ROUND_2_INT((maxTorque / 1023.0));
	if (data > 1023) {
		data = 1023;
	}
	dxl_write_word(servoID, MAX_TORQUE_ADDR, data);
}

DynaServo::alarm DynaServo::getAlarmLedConditions() {
	DynaServo::alarm a;
	int data = dxl_read_byte(servoID, ALARM_LED_ADDR);
	int CommStatus = dxl_get_result();
	if (CommStatus == COMM_RXSUCCESS) {
		a.inputVoltageError = (data & 0x01);
		a.angleLimitError = (data & 0x02);
		a.overHeatingError = (data & 0x04);
		a.rangeError = (data & 0x08);
		a.checksumError = (data & 0x10);
		a.overloadError = (data & 0x20);
		a.instructionError = (data & 0x40);
		return a;
	} else {
		setCommunicationError(CommStatus);
		return a;
	}
}

void DynaServo::setAlarmLedConditions(DynaServo::alarm onOff) {
	uint8_t data = 0x00;
	data |= (onOff.inputVoltageError) ? 0x01 : 0x00;
	data |= (onOff.angleLimitError) ? 0x02 : 0x00;
	data |= (onOff.overHeatingError) ? 0x04 : 0x00;
	data |= (onOff.rangeError) ? 0x08 : 0x00;
	data |= (onOff.checksumError) ? 0x10 : 0x00;
	data |= (onOff.overloadError) ? 0x20 : 0x00;
	data |= (onOff.instructionError) ? 0x40 : 0x00;
	dxl_write_byte(servoID, ALARM_LED_ADDR, data);
}

DynaServo::alarm DynaServo::getAlarmShutdownConditions() {
	DynaServo::alarm a;
	int data = dxl_read_byte(servoID, ALARM_SHUT_ADDR);
	int CommStatus = dxl_get_result();
	if (CommStatus == COMM_RXSUCCESS) {
		a.inputVoltageError = (data & 0x01);
		a.angleLimitError = (data & 0x02);
		a.overHeatingError = (data & 0x04);
		a.rangeError = (data & 0x08);
		a.checksumError = (data & 0x10);
		a.overloadError = (data & 0x20);
		a.instructionError = (data & 0x40);
		return a;
	} else {
		setCommunicationError(CommStatus);
		return a;
	}
}

void DynaServo::setAlamrShutdownConditions(alarm alarm) {
	uint8_t data = 0x00;
	data |= (alarm.inputVoltageError) ? 0x01 : 0x00;
	data |= (alarm.angleLimitError) ? 0x02 : 0x00;
	data |= (alarm.overHeatingError) ? 0x04 : 0x00;
	data |= (alarm.rangeError) ? 0x08 : 0x00;
	data |= (alarm.checksumError) ? 0x10 : 0x00;
	data |= (alarm.overloadError) ? 0x20 : 0x00;
	data |= (alarm.instructionError) ? 0x40 : 0x00;
	dxl_write_byte(servoID, ALARM_SHUT_ADDR, data);
}

bool DynaServo::isTorqueEnabled() {
	int data = dxl_read_byte(servoID, TOR_ENB_ADDR);
	int CommStatus = dxl_get_result();
	if (CommStatus == COMM_RXSUCCESS) {
		return (data == 1);
	} else {
		setCommunicationError(CommStatus);
		return false;
	}
}

void DynaServo::setTorqueEnabled(bool enable) {
	if (enable) {
		torqueShutdown = false;
	} else {
		torqueShutdown = true;
	}
	dxl_write_byte(servoID, TOR_ENB_ADDR, ((enable) ? 0x01 : 0x00));
}

bool DynaServo::isLEDOn() {
	int data = dxl_read_byte(servoID, LED_ON_OFF_ADDR);
	int CommStatus = dxl_get_result();
	if (CommStatus == COMM_RXSUCCESS) {
		return (data == 1);
	} else {
		setCommunicationError(CommStatus);
		return false;
	}
}

void DynaServo::setLEDOn(bool onOff) {
	dxl_write_byte(servoID, LED_ON_OFF_ADDR, ((onOff) ? 0x01 : 0x00));
}

DynaServo::PID DynaServo::getPIDGainValues() {
	PID pid;
	if (isAxServo) {
		throw string("Error: This is not a MX servo.");
	}
	int data = dxl_read_byte(servoID, P_GAIN_ADDR);
	int CommStatus = dxl_get_result();
	if (CommStatus == COMM_RXSUCCESS) {
		pid.proportionalGain = data;
	} else {
		setCommunicationError(CommStatus);
		throw string("Communication Error.");
	}

	data = dxl_read_byte(servoID, I_GAIN_ADDR);
	CommStatus = dxl_get_result();
	if (CommStatus == COMM_RXSUCCESS) {
		pid.integralGain = data;
	} else {
		setCommunicationError(CommStatus);
		throw string("Communication Error.");
	}

	data = dxl_read_byte(servoID, D_GAIN_ADDR);
	CommStatus = dxl_get_result();
	if (CommStatus == COMM_RXSUCCESS) {
		pid.derrivativeGain = data;
	} else {
		setCommunicationError(CommStatus);
		throw string("Communication Error.");
	}
	return pid;
}

void DynaServo::setPIDGainValue(PID pid) {
	if (isAxServo) {
		throw string("Error: This is not a MX servo.");
	}
	dxl_write_byte(servoID, D_GAIN_ADDR, pid.derrivativeGain);
	dxl_write_byte(servoID, I_GAIN_ADDR, pid.integralGain);
	dxl_write_byte(servoID, P_GAIN_ADDR, pid.proportionalGain);
}

bool DynaServo::isAlarmShutdown() {
	if (torqueShutdown) {
		alarm a = getCurrentAlarms();
		alarm as = getAlarmShutdownConditions();
		if (a.angleLimitError && as.angleLimitError) {
			return true;
		}
		if (a.checksumError && as.checksumError) {
			return true;
		}
		if (a.inputVoltageError && as.inputVoltageError) {
			return true;
		}
		if (a.instructionError && as.instructionError) {
			return true;
		}
		if (a.overHeatingError && as.overHeatingError) {
			return true;
		}
		if (a.overloadError && as.overloadError) {
			return true;
		}
		if (a.rangeError && as.rangeError) {
			return true;
		}
		return false;
	} else {
		return isTorqueEnabled();
	}
}

DynaServo::alarm DynaServo::getCurrentAlarms() {
	alarm a;
	bool noOtherError = true;

	if (DynaServo::getAngle() > DynaServo::getCCWAngleLimit()) {
		a.angleLimitError = true;
		noOtherError = false;
	} else if (DynaServo::getAngle() < DynaServo::getCWAngleLimit()) {
		a.angleLimitError = true;
		noOtherError = false;
	} else {
		a.angleLimitError = false;
	}

	if (DynaServo::getVoltage() > DynaServo::getHighVoltageAlarmValue()) {
		a.inputVoltageError = true;
		noOtherError = false;
	} else if (DynaServo::getVoltage() < DynaServo::getLowVoltageAlarmValue()) {
		a.inputVoltageError = true;
		noOtherError = false;
	} else {
		a.inputVoltageError = false;
	}

	if (DynaServo::getTemperatureC() > DynaServo::getTemperatureLimitC()) {
		a.inputVoltageError = true;
		noOtherError = false;
	} else {
		a.inputVoltageError = false;
		noOtherError = false;
	}

	if(!torqueShutdown && isTorqueEnabled() && noOtherError){
		a.overloadError = true;
	} else {
		a.overloadError = false;
	}

	a.checksumError = false; //no way to acceartly check it.
	a.instructionError = false; //no way to acceartly check it.
	a.rangeError = false; //no way to acceartly check it.

	return a;
}

DynaServo::SERVO_TYPE DynaServo::getServoType() {
		int model = DynaServo::getModelNumber();
		switch (model) {
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
		case -1:
			return NONE;
		default:
			return UNKNOWN;
		}
}

string DynaServo::getServoTypeString() {
	switch (DynaServo::getServoType()) {
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

ostream& DynaServo::print(ostream& os) {
	os << "#########################################" << endl;
	os << "ID: " << getID() << endl;
	os << "Servo Type: " << getServoTypeString() << endl;
	os << "Firmware: " << getFirmwareVers() << endl;
	os << "Angle: " << getAngle() << endl;
	os << "Temp F (max: " << getTemperatureLimitF() << "F): " << getTemperatureF() << "F" << endl;
	os << "Voltage (max: " << getHighVoltageAlarmValue() << "V): " << getVoltage() << "V" << endl;
	os << "In alarm state: " << ((DynaServo::isAlarmShutdown())?"TRUE":"FALSE") << endl;
	if(DynaServo::isAlarmShutdown()){
		os << "**********Alarm state**********" << endl;
		alarm a = DynaServo::getCurrentAlarms();
		os << "\tOver Heating Error: " << a.overHeatingError << endl;
		os << "\tOverload Error: " << a.overloadError << endl;
		os << "\tInput Voltage Error: " << a.inputVoltageError << endl;
		os << "\tAngle Limit Error: " << a.angleLimitError << endl;
		os << "\tChecksum Error: " << a.checksumError << endl;
		os << "\tInstruction Error: " << a.instructionError << endl;
		os << "\tValue Range Error: " << a.rangeError << endl;
	}
	return os;
}
