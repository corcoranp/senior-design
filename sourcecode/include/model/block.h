/*
 * block.h
 *
 *  Created on: Jan 16, 2016
 *      Author: Peter
 */

#ifndef BLOCK_H_
#define BLOCK_H_

#include "../enums.h"
//#include "../globals.h"

using namespace std;

namespace blaze {

	class block {
		private:
			Color myColor;
			Size mySize;
		public:
			block (Color blockColor, Size blockSize){
				myColor = blockColor;
				mySize = blockSize;
			}

			/*
			 * Method for getting the block's color
			 */
			Color getColor();
			Size getSize();
	};

}


#endif /* BLOCK_H_ */
