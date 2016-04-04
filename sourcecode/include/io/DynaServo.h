/*
 * Dynamixel.h
 *
 *  Created on: Mar 16, 2016
 *      Author: Tobie
 */

#ifndef DYNA_SERVO_H_
#define DYNA_SERVO_H_

#include "../BasicFunctions.h"
#include <termio.h>
#include "dynamixel.h"
#include <iostream>

using namespace std;

#define ANGLE_INCREASE 2

class DynaServo {
public:
	enum ERROR_CODE {
		NO_ERROR,
		VOLTAGE,
		OVERHEAT,
		ANGLE,
		RANGE,
		OVERLOAD,
		CONNECTION,
		ID,
		BAUDRATE,
		RXCORRUPT,
		RXTIMEOUT,
		RXWAITING,
		RXFAIL,
		TXERROR,
		TXFAIL,
		UNKNOWN_COMM
	};

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

	typedef struct alarm_t{
		bool instructionError;
		bool overloadError;
		bool checksumError;
		bool rangeError;
		bool overHeatingError;
		bool angleLimitError;
		bool inputVoltageError;
	} alarm;

	typedef struct PID_t{
		uint8_t proportionalGain;
		uint8_t integralGain;
		uint8_t derrivativeGain;
	} PID;

	DynaServo(int serID, long baud);
	DynaServo(const DynaServo& orig);
	virtual ~DynaServo();
	ERROR_CODE getErrorCode();
	int getModelNumber();
	int getFirmwareVers();
	int getID();
	void setID(int id);
	long getBaudrate();
	void setBaudrate(long bps);
	int getTransDelay();
	void setTransDelay(int usec);
	double getCWAngleLimit();
	void setCWAngleLimit(double angle);
	double getCCWAngleLimit();
	void setCCWAngleLimit(double angle);
	double getTemperatureLimitF();
	double getTemperatureLimitC();
	double getLowVoltageAlarmValue();
	double getHighVoltageAlarmValue();
	double getMaxTorqueLimit();
	void setMaxTorqueLimit(double maxTorque);
	alarm getAlarmLedConditions();
	void setAlarmLedConditions(alarm onOff);
	alarm getAlarmShutdownConditions();
	void setAlamrShutdownConditions(alarm alarm);
	//TODO: add code to set servo operation mode
	//TODO: add code for multi-turn mode.
	//TODO: add code to adjust resolution for multi-turn mode
	bool isTorqueEnabled();
	void setTorqueEnabled(bool enable);
	bool isLEDOn();
	void setLEDOn(bool onOff);
	PID getPIDGainValues();
	void setPIDGainValue(PID pid);
	void setAngle(double angle);
	double getAngle();
	double getSpeed();
	void setSpeed(double rpm);
	double getVoltage();
	double getTemperatureF();
	double getTemperatureC();
	bool isMoving();
	bool isConnected();
	bool isAlarmShutdown();
	alarm getCurrentAlarms();
	DynaServo::SERVO_TYPE getServoType();
	string getServoTypeString();

	ostream& print(ostream& os);


    /**
     * Get the current servo angle in radians.
     * @return return the current angle that the servo is set to in radians.
     */
    double getRadians(void);

    /**
     * Get the maximum angle the servo can physically turn in degrees.
     * @return The maximum angle the servo can physically turn in degrees
     */
    int getMaxAngle();

private:
	int servoID;
	long baudRate;
	bool connected;
	ERROR_CODE err;
	SERVO_TYPE type;
	bool isAxServo;
	bool torqueShutdown;

	void setCommunicationError(int error);
};

#endif /* SERVOS_DYNAMIXEL_H_ */
