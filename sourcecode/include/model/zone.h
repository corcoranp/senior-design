/*
 * Zone.h
 * By: Peter Corcoran
 * Copyright: © 2015
 *
 *	Zones are areas of the field that the robot interacts with
 *	Zones can:
 *	- accept container deliveries
 *	- hold containers waiting to be picked up
 *
 *	Zones have:
 *	- a location
 *	- a size
 *	- a type
 *	- a color
 *
 * Jun 26, 2015 - Initial Coding
 *
 */

#ifndef INCLUDE_ZONE_H_
#define INCLUDE_ZONE_H_


#include <vector>
#include <iostream>
#include <string>
#include "../enums.h"
#include "inventory.h"


using namespace std;

namespace blaze {
	class zone{
		private:
			inventory zoneInventory;

		public:
			zone(){
				//upon creation of the zone, create an inventory object.
				//zoneInventory = new inventory();

			}
			/*
			 * Constructor that accepts an inventory config.
			 */
			zone(string config){
				//create new inventory:
				//zoneInventory	 = new inventory();

				//parse the inventory config:
				std::istringstream ss(config);
				std::vector<int> vect;
				int di;

				while (ss >> di)
				{
					vect.push_back(di);
					if (ss.peek() == ',')
						ss.ignore();
				}


				//vect[0]=blue 5"
				zoneInventory.addBlocks(vect[0], BLUE, LARGE);
				//vect[1]=blue 2.5"
				zoneInventory.addBlocks(vect[1], BLUE, SMALL);
				//vect[2]=green 5"
				zoneInventory.addBlocks(vect[2], GREEN, LARGE);
				//vect[3]=green 2.5"
				zoneInventory.addBlocks(vect[3], GREEN, SMALL);
				//vect[4]=red 5"
				zoneInventory.addBlocks(vect[4], RED, LARGE);
				//vect[5]=red 2.5"
				zoneInventory.addBlocks(vect[5], RED, SMALL);
				//vect[6]=yellow 5"
				zoneInventory.addBlocks(vect[6], YELLOW, LARGE);
				//vect[7]=yellow 2.5"
				zoneInventory.addBlocks(vect[7], YELLOW, SMALL);

			}

			inventory& getInventory();




	};

}

#endif /* INCLUDE_ZONE_H_ */
