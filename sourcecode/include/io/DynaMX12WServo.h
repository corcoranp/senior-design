/*
 * Dynamixel.h
 *
 *  Created on: Mar 16, 2016
 *      Author: Tobie
 */

#ifndef SERVOS_DYNAMIXEL_H_
#define SERVOS_DYNAMIXEL_H_

#include <termio.h>
#include "../../include/controllers/dynamixel.h"

#ifndef ROUND_2_INT
#define ROUND_2_INT(f) ((int)(f >= 0.0 ? (f + 0.5) : (f - 0.5)))
#endif

#define ANGLE_INCREASE 2

enum ERROR_CODE {
	NONE,
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

class DynaMX12WServo {
public:
	DynaMX12WServo();
	DynaMX12WServo(int devID, int serID, long baud);
	DynaMX12WServo(const DynaMX12WServo& orig);
	virtual ~DynaMX12WServo();
	ERROR_CODE getErrorCode();
	double getVoltage();
	double getTemperatureF();
	double getTemperatureC();
	double getAngle();
	double getSpeed();
	void setAngle(double angle);
	void setSpeed(double rpm);
	int getModelNumber();
	int getFirmwareVers();
	long getBaudrate();
	void setBaudrate(long bps);
	int getTransDelay();
	void setTransDelay(int usec);
	int getID();
	void setID(int id);
	bool connect();
	bool isConnected();
	void closeConnection();
	bool isMoving();
	void setSingleMove(bool move);
	bool getSingleMove();
	bool moveServo();
	void setServoPosition(double pos);
	double getPosition();

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
	int deviceID;
	long baudRate;
	bool connected;
	ERROR_CODE err;
	bool singleMove;
	double nextAng;

	void setCommunicationError(int error);
	void setServoAngle(double angle);
};

#endif /* SERVOS_DYNAMIXEL_H_ */
