/*
 * ArmConstraint.h
 *
 *  Created on: Mar 25, 2016
 *      Author: Tobie
 */

#ifndef ARMCONSTRAINT_H_
#define ARMCONSTRAINT_H_
#include "../io/DynaServo.h"
#include <vector>

using namespace std;

class ArmConstraint {
public:
	typedef struct ServoCon_t {
		double minAngle;
		double maxAngle;
		int id;
	} ServoCon;

	ArmConstraint();
	virtual ~ArmConstraint();
	ServoCon getServoConstraint(int id) throw (string);
	void setServoConstraint(ServoCon con);
	bool checkAngle(DynaServo *servo, double angle);
	bool isMaxAngle(DynaServo *servo, double angle);
	bool isMinAngle(DynaServo *servo, double angle);

private:
	vector<ServoCon> cons;
};

#endif /* ARMCONSTRAINT_H_ */
