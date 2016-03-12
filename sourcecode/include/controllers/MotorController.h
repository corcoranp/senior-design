/*
 * MotorController.h
 *
 *  Created on: Oct 4, 2015
 *      Author: Peter
 */

#ifndef INCLUDE_CONTROLLERS_MOTORCONTROLLER_H_
#define INCLUDE_CONTROLLERS_MOTORCONTROLLER_H_

namespace blaze {

	class MotorController {
	public:

		bool isEnabled;

		/*
		 * Single Motor Control
		 */

		void engage(int motorIndex, int speed);
		void brake(int motorIndex);
		void slower(int motorIndex);
		void faster(int motorIndex);

		/*
		 * Dual Motor Control
		 */
		void forward();
		void forward(int rate);
		void forward(int rate, int degrees);

		void backward();
		void backward(int rate);
		void backward(int rate, int degrees);


		void stop();
		void stop(int rate);


	private:

	};

}
#endif /* INCLUDE_CONTROLLERS_MOTORCONTROLLER_H_ */
