/*
 * starter.h
 *
 *  Created on: Mar 11, 2016
 *      Author: Peter
 */

#ifndef STARTER_H_
#define STARTER_H_

namespace blaze {

class starter {

private:
	GPIO *gpio_start, *gpio_stop;


public:
	starter(GPIO *gpio_start, GPIO *gpio_stop);
	starter(int gpio_start, int gpio_stop);

	virtual bool requestStart();
	virtual bool requestStop();

	virtual ~starter();
};

} /* namespace blaze */

#endif /* STARTER_H_ */
