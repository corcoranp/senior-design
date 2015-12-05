/*
 * fileSerializer.h
 *
 *  Created on: Nov 18, 2015
 *      Author: Peter
 */

#ifndef FILESERIALIZER_H_
#define FILESERIALIZER_H_



class fileSerializer {

public:
	static void write(char* filename, blaze::config& cfg);
	static blaze::config& load(char* filename);

};



#endif /* FILESERIALIZER_H_ */
