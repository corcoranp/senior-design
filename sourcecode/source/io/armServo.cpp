/*
 * armServo.cpp
 *
 *  Created on: Mar 11, 2016
 *      Author: Peter
 */
#include <stdio.h>
#include <stdlib.h>
#include "../../include/io/armServo.h"

namespace blaze {


// Control table address
#define P_GOAL_POSITION_L	30
#define P_GOAL_POSITION_H	31
#define P_PRESENT_POSITION_L	36
#define P_PRESENT_POSITION_H	37
#define P_MOVING		46
#define P_ID	3


// Defulat setting
#define DEFAULT_BAUDNUM		1 // 1Mbps
#define DEFAULT_ID		1

/*
void PrintCommStatus(int CommStatus);
void PrintErrorCode(void);
*/



armServo::armServo() {
	// TODO Auto-generated constructor stub


	//Code from Demo Need to replaces
	 int id;

		/* istringstream iis(argv[1]);
		 int val;
		 if((iis >> val) && iis.eof()){
			 id = val;
		 } else
		 {
			 id = 0;
		 }
*/
		/* int baudnum = 1;
		 	int GoalPos[2] = {0, 1023};
		 	//int GoalPos[2] = {0, 4095}; // for Ex series
		 	int index = 0;
		 	int deviceIndex = 0;
		 	int Moving, PresentPos;
		 	int CommStatus;

		 	printf( "\n\nRead/Write example for Linux\n\n" );
		 	///////// Open USB2Dynamixel ////////////
		 	if( dxl_initialize(deviceIndex, baudnum) == 0 )
		 	{
		 		printf( "Failed to open USB2Dynamixel!\n" );
		 		printf( "Press Enter key to terminate...\n" );
		 		getchar();
		 		return ;
		 	}
		 	else
		 		printf( "Succeed to open USB2Dynamixel!\n" );
		 		*/

	//	 	cout << "ID: " + to_string(dxl_read_word( id, 0 ) ) << endl;
	//	 	dxl_write_byte( 253, 0x07, 1 );



	int i;
	for (i=0; i<=255; i++){
		 	//cout << "Index (" + to_string(i) + "): " + to_string(dxl_read_byte( 1, i  ) ) << endl;
	}
	/*
		 	while(1)
		 	{
		 		printf( "Press Enter key to continue!(press ESC and Enter to quit)\n" );
		 		if(getchar() == 0x1b)
		 			break;

		 		// Write goal position
		 		dxl_write_word( id, P_GOAL_POSITION_L, GoalPos[index] );
		 		do
		 		{
		 			// Read present position
		 			PresentPos = dxl_read_word( id, P_PRESENT_POSITION_L );
		 			CommStatus = dxl_get_result();

		 			if( CommStatus == COMM_RXSUCCESS )
		 			{
		 				printf( "%d   %d\n",GoalPos[index], PresentPos );
		 				PrintErrorCode();
		 			}
		 			else
		 			{
		 				PrintCommStatus(CommStatus);
		 				break;
		 			}

		 			// Check moving done
		 			Moving = dxl_read_byte( id, P_MOVING );
		 			CommStatus = dxl_get_result();
		 			if( CommStatus == COMM_RXSUCCESS )
		 			{
		 				if( Moving == 0 )
		 				{
		 					// Change goal position
		 					if( index == 0 )
		 						index = 1;
		 					else
		 						index = 0;
		 				}

		 				PrintErrorCode();
		 			}
		 			else
		 			{
		 				PrintCommStatus(CommStatus);
		 				break;
		 			}
		 		}while(Moving == 1);
		 	}
	*/

		 	// Close device
		/* 	dxl_terminate();
		 	printf( "Press Enter key to terminate...\n" );
		 	getchar();
		 	return;

*/
		 	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~





}

armServo::~armServo() {
	// TODO Auto-generated destructor stub
}

} /* namespace blaze */


/*
void PrintCommStatus(int CommStatus)
{
	switch(CommStatus)
	{
	case COMM_TXFAIL:
		printf("COMM_TXFAIL: Failed transmit instruction packet!\n");
		break;

	case COMM_TXERROR:
		printf("COMM_TXERROR: Incorrect instruction packet!\n");
		break;

	case COMM_RXFAIL:
		printf("COMM_RXFAIL: Failed get status packet from device!\n");
		break;

	case COMM_RXWAITING:
		printf("COMM_RXWAITING: Now recieving status packet!\n");
		break;

	case COMM_RXTIMEOUT:
		printf("COMM_RXTIMEOUT: There is no status packet!\n");
		break;

	case COMM_RXCORRUPT:
		printf("COMM_RXCORRUPT: Incorrect status packet!\n");
		break;

	default:
		printf("This is unknown error code!\n");
		break;
	}
}

// Print error bit of status packet
void PrintErrorCode()
{
	if(dxl_get_rxpacket_error(ERRBIT_VOLTAGE) == 1)
		printf("Input voltage error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_ANGLE) == 1)
		printf("Angle limit error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_OVERHEAT) == 1)
		printf("Overheat error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_RANGE) == 1)
		printf("Out of range error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_CHECKSUM) == 1)
		printf("Checksum error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_OVERLOAD) == 1)
		printf("Overload error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_INSTRUCTION) == 1)
		printf("Instruction code error!\n");
}

*/

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



