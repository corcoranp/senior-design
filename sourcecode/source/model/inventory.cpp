/*
 * inventory.cpp
 *
 *  Created on: Jan 16, 2016
 *      Author: Peter
 */



#include "../../include/model/inventory.h"
#include "../../include/model/block.h"

using namespace std;
using namespace blaze;


/*
 * Function adds a single block to the inventory
 */
void inventory::add(block b){
	//adds the block to the tracking vector
	blocks.push_back(b);
}

/*
 * Function adds a number of blocks of the same type to the inventory
 */
void inventory::addBlocks(int count, Color c, Size s){

	for(int i = 1; i <= count; ++i){
		block b(c, s);
		add(b);
	}
}
