/*
 * rails.h
 *
 *  Created on: Jan 16, 2016
 *      Author: Peter
 */

#ifndef RAILS_H_
#define RAILS_H_


#include "rail.h";

using namespace std;
using namespace blaze;

namespace blaze {

	class rails {
	private:
		Color myColor;
		rail& car1;
		rail& car2;
		rail& car3;
		rail& car4;

	public:

		void setCarOne(rail& r);
		void setCarTwo(rail& r);
		void setCarThree(rail& r);




	};
}


#endif /* RAILS_H_ */
