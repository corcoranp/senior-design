/*
 * pdata.h
 *
 *  Created on: Mar 16, 2016
 *      Author: Peter
 */

#ifndef PDATA_H_
#define PDATA_H_

#include <algorithm>
#include <mutex>

using namespace std;

class pdata {


	pdata();
	virtual ~pdata();

public:
	static pdata *s_instance;
	//GET singleton class
	static pdata *instance(){
		if(!s_instance){
			s_instance = new pdata;
		}
		return s_instance;
	}

	void updateElement(int index, double value);
	static double readElement(int index);
};

#endif /* PDATA_H_ */
