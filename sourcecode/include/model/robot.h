/*
 * robot.h
 *
 *  Created on: Jan 16, 2016
 *      Author: Peter
 */

#ifndef ROBOT_H_
#define ROBOT_H_

#include "storage.h"

using namespace std;

namespace blaze {

class robot {
	private:
		storage botStorage;

	public:

		storage getStorage();

};

}


#endif /* ROBOT_H_ */
