/*
 * Senior Design Project Bootloader
 * By: Peter Corcoran
 * Created: June 24, 2015
 *
 * Initial c++ coding for 2015-2016 UAB EE Senior Design
 *
 */
#include <iostream>
#include "../include/QRCode.h"


using namespace std;


 int main(void)
{
	 cout << "Bootloader Started" << endl;

	 QRCode q;

	 q.getCode();

	 Color red = RED; //example ENUM value..

    return 0;
}
