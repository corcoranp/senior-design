/*
 * port.h
 *
 *  Created on: Jan 16, 2016
 *      Author: Peter
 */

#ifndef PORT_H_
#define PORT_H_

#include "../enums.h"
#include "../system/console.h"
#include "zone.h"
#include "rail.h"
#include "../globals.h"

using namespace std;

namespace blaze {

class port {
	private:
		zone zoneA;
		zone zoneB;
		zone zoneC;


	public:
		port(PortConfig pc){
			//upon creation of the port, create default inventory items.
			//Create zone A pass in inventory config
			//zoneA = ((pc == A) ? new zone(PA_ZONEA_INVENTORY) : new zone(PB_ZONEA_INVENTORY));
			//zoneB = ((pc == A) ? new zone(PA_ZONEB_INVENTORY) : new zone(PB_ZONEB_INVENTORY));
			//zoneC = ((pc == A) ? new zone(PA_ZONEC_INVENTORY) : new zone(PB_ZONEC_INVENTORY));
		}

		zone& getZoneA();
		zone& getZoneB();
		zone& getZoneC();
		zone& getBoat();
		zone& getTruck();
		rail& getRailOne();
		rail& getRailTwo();
		rail& getRailThree();
		rail& getRailFour();

};

}



#endif /* PORT_H_ */
