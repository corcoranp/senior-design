/*
 * positionMonitor.h
 *
 *  Created on: Mar 20, 2016
 *      Author: Peter
 */

#ifndef POSITIONMONITOR_H_
#define POSITIONMONITOR_H_

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "../../include/system/thread.h"
#include "../../include/system/workqueue.h"

class positionMonitor : public Thread{
public:
	positionMonitor();
	virtual ~positionMonitor();

	//workqueue<> queue
};

#endif /* POSITIONMONITOR_H_ */
