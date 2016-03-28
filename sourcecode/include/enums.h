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
	enum WALL_FOLLOWING {RIGHT, LEFT, DISABLED};

	enum Face {Right, Front, Left, Rear };
	enum DistType {Avg, Min, Max, Theta};
	enum MOVEMENT {FORWARD, REVERSE};

	enum Zones {AZ, BZ, CZ, TRUCK, BOAT, RAIL_BLUE, RAIL_GREEN, RAIL_RED, RAIL_YELLOW, UNDEFINED_ZONE};

	enum QUADRANT { I, II, III, IV, NONE};
	enum CARDINAL { NORTH, SOUTH, EAST, WEST};

}



#endif /* ENUMS_H_ */
