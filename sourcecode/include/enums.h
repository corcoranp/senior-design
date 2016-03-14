/*
 * enums.h
 *
 *  Created on: Jan 17, 2016
 *      Author: Peter
 */

#ifndef ENUMS_H_
#define ENUMS_H_

namespace blaze{

	enum Color {BLUE, GREEN, YELLOW, RED };
	enum PortConfig {A, B, UNDEFINED };
	enum Size { SMALL, LARGE};
	enum DIRECTION{ CLOCKWISE, ANTICLOCKWISE };
	enum SPEED {EIGHTH, QUARTER, THREE_EIGHTH, HALF, FIVE_EIGHTH, THREE_QUARTER, SEVEN_EIGHTH, FULL };
	enum WALL_FOLLOWING {RIGHT, LEFT};
}



#endif /* ENUMS_H_ */
