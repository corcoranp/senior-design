/*
 * fileSerializer.cpp
 *
 *  Created on: Nov 18, 2015
 *      Author: Peter
 */


#include <string>
#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <fstream>
#include "../include/blazecore.h"
#include "../include/system/fileSerializer.h"

using namespace std;
using namespace blaze;


void fileSerializer::write(char* filename, config& cfg){

		ofstream outfile;

		outfile.open(filename, ofstream::trunc);
		outfile.write((char*)&cfg, sizeof(blaze::config));
		outfile.close();

}

blaze::config& fileSerializer::load(char* filename){
	blaze::config cfg;
	ifstream infile;

	infile.open(filename);
	infile.read((char*)&cfg, sizeof(blaze::config));
	infile.close();

	return cfg;
}
