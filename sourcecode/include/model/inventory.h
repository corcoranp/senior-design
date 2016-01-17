/*
 * inventory.h
 *
 *  Created on: Jan 16, 2016
 *      Author: Peter
 */

#ifndef INVENTORY_H_
#define INVENTORY_H_


#include <vector>
#include <sstream>
#include <stdio.h>
#include "../enums.h"
#include "block.h"

using namespace std;

namespace blaze {

class inventory {
	private:
		vector<block> blocks;

	public:
		inventory(){

		}
		void add(block b);
		void addBlocks(int count, Color c, Size s);


};

}



#endif /* INVENTORY_H_ */
