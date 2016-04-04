/*
 * ArmConstraint.cpp
 *
 *  Created on: Mar 25, 2016
 *      Author: Tobie
 */

#include "../../include/Arm/ArmConstraint.h"

ArmConstraint::ArmConstraint() {
	//Nothing to do
}

ArmConstraint::~ArmConstraint() {
	//Nothing to do
}

bool ArmConstraint::checkAngle(DynaServo *servo, double angle) {
	int id = servo->getID();
	if (id < 0) { //invalid id
		return false;
	}
	if (id > 254) { //invalid id
		return false;
	}
	for (unsigned int i = 0; i < cons.size(); i++) {
		if (cons[i].id == id) {
			if (angle > cons[i].maxAngle) {
				return false;
			}
			if (angle < cons[i].minAngle) {
				return false;
			}
			return true;
		}
	}
	return false;
}

ArmConstraint::ServoCon ArmConstraint::getServoConstraint(int id) throw (string) {
	for (unsigned int i = 0; i < cons.size(); i++) {
		if (cons[i].id == id) {
			return cons[i];
		}
	}
	ostringstream s;
	s << "Illegal ID: " << id;
	throw s.str();
}

void ArmConstraint::setServoConstraint(ServoCon con) {
	for (unsigned int i = 0; i < cons.size(); i++) {
		if (cons[i].id == con.id) {
			cons[i] = con;
			return;
		}
	}
	cons.push_back(con);
}

bool ArmConstraint::isMaxAngle(DynaServo *servo, double angle) {
	int id = servo->getID();
	if (id < 0) { //invalid id
		return false;
	}
	if (id > 254) { //invalid id
		return false;
	}
	for (unsigned int i = 0; i < cons.size(); i++) {
		if (cons[i].id == id) {
			if (angle > cons[i].maxAngle) {
				return true;
			}
			return false;
		}
	}
	return false;
}

bool ArmConstraint::isMinAngle(DynaServo *servo, double angle) {
	int id = servo->getID();
	if (id < 0) { //invalid id
		return false;
	}
	if (id > 254) { //invalid id
		return false;
	}
	for (unsigned int i = 0; i < cons.size(); i++) {
		if (cons[i].id == id) {
			if (angle < cons[i].minAngle) {
				return true;
			}
			return false;
		}
	}
	return false;
}
