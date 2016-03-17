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
	enum DIRECTION{ CLOCKWISE, ANTICLOCKWISE }; //clockwise is forward...
	enum SPEED {EIGHTH, QUARTER, THREE_EIGHTH, HALF, FIVE_EIGHTH, THREE_QUARTER, SEVEN_EIGHTH, FULL };
	enum WALL_FOLLOWING {RIGHT, LEFT};

	enum Face {Front, Left, Right, Back, Front_L, Front_C, Front_R, Back_L, Back_C, Back_R};
	enum DistType {Avg, Min, Max, Theta};
	enum MOVEMENT {FORWARD, REVERSE};
}



#endif /* ENUMS_H_ */
