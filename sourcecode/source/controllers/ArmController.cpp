
#include <iostream>

#include "../../include/controllers/ArmController.h"
#include "../../include/globals.h"
#include "../../include/controllers/dynamixel.h";

using namespace std;
using namespace blaze;




#define P_GOAL_POSITION_L	30
#define P_GOAL_POSITION_H	31
#define P_PRESENT_POSITION_L	36
#define P_PRESENT_POSITION_H	37
#define P_MOVING		46


ArmController::ArmController() {
	// TODO Auto-generated constructor stub

}

ArmController::~ArmController() {
	// TODO Auto-generated destructor stub
}


void ArmController::printServoIds(){
	int Moving, PresentPos, elbowP;
	int CommStatus;
	int baudnum = 1;

	if( dxl_initialize(1, baudnum) == 0 )
	{
		printf( "Failed to open USB2Dynamixel!\n" );
		printf( "Press Enter key to terminate...\n" );
		getchar();
		return ;
	}
	else
		printf( "Succeed to open USB2Dynamixel!\n" );

/*
 * int ARM_SHOULDER_ID = 20;
	int ARM_ELBOW1_ID = 21;
	int ARM_ELBOW2_ID = 22;
	int ARM_ELBOW3_ID = 23;
	int ARM_WRIST_ID = 24;
	int ARM_HAND_ID = 25;
 */


while (1){


	PresentPos = dxl_read_word( ARM_SHOULDER_ID, P_PRESENT_POSITION_L );
	cout << "Present Position Shoulder: " + to_string(PresentPos) << endl;

	elbowP = dxl_read_word( ARM_ELBOW1_ID, P_PRESENT_POSITION_L );
	cout << "Present Position Elbow 1: " + to_string(elbowP) << endl;

	PresentPos = dxl_read_word( ARM_ELBOW2_ID, P_PRESENT_POSITION_L );
	cout << "Present Position Elbow 2: " + to_string(PresentPos) << endl;

	PresentPos = dxl_read_word( ARM_ELBOW3_ID, P_PRESENT_POSITION_L );
	cout << "Present Position Elbow 3: " + to_string(PresentPos) << endl;

	PresentPos = dxl_read_word( ARM_WRIST_ID, P_PRESENT_POSITION_L );
	cout << "Present Position Wrist: " + to_string(PresentPos) << endl;

	PresentPos = dxl_read_word( ARM_HAND_ID, P_PRESENT_POSITION_L );
	cout << "Present Position Hand: " + to_string(PresentPos) << endl;

	CommStatus = dxl_get_result();


	cout << "Present Position: " + to_string(elbowP - 10) << endl;


	// Close device

	printf( "Press Enter key to terminate...\n" );
	getchar();

	int GoalPos[2] = {0, 1023};
	dxl_write_word( ARM_ELBOW2_ID, P_GOAL_POSITION_L, GoalPos[elbowP + 10] );

}
	dxl_terminate();
}
