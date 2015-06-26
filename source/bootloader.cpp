/*
 * Senior Design Project Bootloader
 * By: Peter Corcoran
 * Created: June 24, 2015
 *
 * Initial c++ coding for 2015-2016 UAB EE Senior Design
 *
 */
#include <iostream>
#include <string>
#include <map>
#include "../include/robotcore.h"
#include "../include/qrcode.h"
#include "../include/zone.h"

/*
 * Namespace Declaration
 */
using namespace std;

/*
 * Function definitions
 */
void print(string);

/*
 * Robot Bootloader Main Control
 *
 */
 int main(void)
{
	 print("EE498 - Senior Design");
	 cout << "Bootloader Started" << endl;

	 QRCode q;

	 q.getCode();

	 Color red = RED; //example ENUM value..

	 map <Color, string> c;


	 cout << c[RED];


	 Zone z;
	 cout << z.color();

    return 0;
}


 void print (string msg){
	 cout << msg << endl;
 }
