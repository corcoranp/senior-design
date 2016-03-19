/*
 * indicator.h
 *
 *  Created on: Mar 11, 2016
 *      Author: Peter
 */

#ifndef INDICATOR_H_
#define INDICATOR_H_

#include "../../include/io/gpio.h"

namespace blaze {

class indicator {
public:
	enum LED {TOP, SECOND, THRID, BOTTOM};
private:
	GPIO *gpio_led1, *gpio_led2, *gpio_led3, *gpio_led4;

public:
	indicator(GPIO *gpio_led1, GPIO *gpio_led2, GPIO *gpio_led3, GPIO *gpio_led4);
	indicator(int gpio_led1, int gpio_led2, int gpio_led3, int gpio_led4);

	virtual void blink(int ms, LED led);
	virtual void started();
	virtual void motor1running(bool isrunning);
	virtual void motor2running(bool isrunning);
	virtual void errorState();

	virtual void on(int light);
	virtual void off(int light);

	virtual ~indicator();


};

} /* namespace blaze */

#endif /* INDICATOR_H_ */
