/*
 * NavigationController.h
 *
 *  Created on: Mar 12, 2016
 *      Author: Peter
 */

#ifndef NAVIGATIONCONTROLLER_H_
#define NAVIGATIONCONTROLLER_H_

#include "../enums.h"
#include "../io/lidarIO.h"

namespace blaze {

class NavigationController {
public:
	NavigationController();
	virtual ~NavigationController();

	static PortConfig determinePort();
	void moveToFirstPosition();

};

} /* namespace blaze */

#endif /* NAVIGATIONCONTROLLER_H_ */
