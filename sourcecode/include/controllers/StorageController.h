/*
 * StorageController.h
 *
 *  Created on: Oct 4, 2015
 *      Author: Peter
 */

#ifndef INCLUDE_CONTROLLERS_STORAGECONTROLLER_H_
#define INCLUDE_CONTROLLERS_STORAGECONTROLLER_H_

#include "../enums.h"
#include "../io/pwm.h"

namespace blaze {

class StorageController {
public:
	string pinName;
	StorageController(string pwmName, int drivepos, int deliverypos, int loadpos);
	virtual ~StorageController();

	void setDrivePostion();
	void setDeliveryPosition();
	void setLoadPosition();
	void setRestPosition();

private:
	unsigned int m_drivepos, m_deliverypos, m_loadpos;

};

} /* namespace blaze */
#endif /* INCLUDE_CONTROLLERS_STORAGECONTROLLER_H_ */
