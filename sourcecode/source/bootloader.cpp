/*
 * Senior Design Project Bootloader
 * By: Peter Corcoran
 * Created: June 24, 2015
 *
 * Initial c++ coding for 2015-2016 UAB EE Senior Design
 *
 */



//Needed for threading
//#include <thread>
//Needed for threading
#include <stdio.h>

#include "../include/robotcore.h"
#include "../include/qrcode.h"
#include "../include/zone.h"


#include "../include/sysops.h"

/*
 * Namespace Declaration
 */
using namespace std;

void print(string);

/*
 * BlazeBot Bootloader Main Control
 *
 * A bootloader is a program that loads an operating system
 * when a computer is turned on, similarly the BlazeBot Bootloader
 * loads the robot's main system's operations, or sysops.
 *
 *
 * main is the main program's loop, sysops isn't loaded at start up
 * in case there is a reason to wait, or preloading of modules that
 * need to be done, then sysops.start() can be executed, and extended
 * to inject any system dependencies/settings.
 */
 int main(void)
{
	 print("EE498 - Senior Design");
	 print("Bootloader Started");
/*
	 QRCode q;


	 q.getCode();

	 Color red = RED; //example ENUM value..

	 map <Color, string> c;


	 cout << c[RED];


	 Zone z;
	 cout << z.color();

*/
	 //SYSOPS STARTUP~~~~~~~~~~~~~~~~~~~~~~~



	 //thread_create(&thread_id,NULL,(sysops::entry),new sysops());
    return 0;
}


 void print (string msg){
	 cout << msg << endl;
 }
