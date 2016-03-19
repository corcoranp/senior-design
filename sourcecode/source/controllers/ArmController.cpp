
#include <iostream>

#include "../../include/system/console.h"
#include "../../include/controllers/ArmController.h"
#include "../../include/globals.h"
#include "../../include/io/StepperMotor.h"
#include "../../include/controllers/dynamixel.h"
#include "../../include/io/DynaMX12WServo.h"


using namespace std;
using namespace blaze;




#define P_GOAL_POSITION_L	30
#define P_GOAL_POSITION_H	31
#define P_PRESENT_POSITION_L	36
#define P_PRESENT_POSITION_H	37
#define P_MOVING		46


ArmController::ArmController(StepperMotor *sc) {
	// TODO Auto-generated constructor stub

	stepper = sc;


	baudRate = 115200;
	int index = 0;
	int CommStatus;

	console::debug( "\n\nRead/Write example for Linux\n\n" );

	uint8_t baudnum = ((2000000 / baudRate) - 1) & 0xFF;
		if (dxl_initialize(1, baudnum) == 0) {
			console::debug("Failed to connect to arm");
			isConnected = false;
		} else {
			console::debug("Arm is connected");
			isConnected = true;
		}

		this->servo20 = DynaMX12WServo(1,20,115200);
		this->servo21 = DynaMX12WServo(1,21,115200);
		this->servo22 = DynaMX12WServo(1,22,115200);
		this->servo23 = DynaMX12WServo(1,23,115200);
		this->servo24 = DynaMX12WServo(1,24,115200);
		this->servo25 = DynaMX12WServo(1,25,115200);

}

ArmController::~ArmController() {
	// TODO Auto-generated destructor stub


}

void ArmController::init(){
	//setup Arm/Stepper/Switches

	//Stepper Initialization
	//this->stepper = StepperMotor(gpio_stepper_ms1,gpio_stepper_ms2,gpio_stepper_ms3,gpio_stepper_step,gpio_stepper_sleep, gpio_stepper_dir, gpio_stepper_en, gpio_stepper_lsw, gpio_stepper_rsw, 60, 200);
}

void ArmController::startup(){
	this->isMoving = true;
	console::debug("Arm Start move....");

		//this->servo23 = DynaMX12WServo(1,23,115200);
		this->servo20.connect();

/*
		this->servo23.setSpeed(50);
		this->servo23.setServoPosition(815);
		this->servo22.setSpeed(50);
	*/

		console::debug("Servo 20 MaxAngle: " + to_string(this->servo20.getPosition()));
		console::debug("Servo 21 MaxAngle: " + to_string(this->servo21.getPosition()));
		console::debug("Servo 22 MaxAngle: " + to_string(this->servo22.getPosition()));
		console::debug("Servo 23 MaxAngle: " + to_string(this->servo23.getPosition()));
		console::debug("Servo 24 MaxAngle: " + to_string(this->servo24.getPosition()));
		console::debug("Servo 25 MaxAngle: " + to_string(this->servo25.getPosition()));

		console::debug("Servo 20 Temp: " + to_string(this->servo20.getTemperatureF()));
		console::debug("Servo 21 Temp: " + to_string(this->servo21.getTemperatureF()));
		console::debug("Servo 22 Temp: " + to_string(this->servo22.getTemperatureF()));
		console::debug("Servo 23 Temp: " + to_string(this->servo23.getTemperatureF()));
		console::debug("Servo 24 Temp: " + to_string(this->servo24.getTemperatureF()));
		console::debug("Servo 25 Temp: " + to_string(this->servo25.getTemperatureF()));


		this->servo23.closeConnection();

		this->isMoving = false;



}
/*
void ArmController::setArmPos(int s21, int s22, int s23, s24){




	console::debug("Servo 20: " + this->servo20.getTemperatureF());
	console::debug("Servo 21: " + this->servo21.getTemperatureF());
	console::debug("Servo 22: " + this->servo22.getTemperatureF());
	console::debug("Servo 23: " + this->servo23.getTemperatureF());
	console::debug("Servo 24: " + this->servo24.getTemperatureF());
	console::debug("Servo 25: " + this->servo25.getTemperatureF());

}*/



void ArmController::stopNow(){

	//Stop stepper motors
	stepper->disable();
	stepper->sleep();
}

void ArmController::moveStepper(){

	stepper->setDirection(StepperMotor::CLOCKWISE);
	stepper->setStepMode(StepperMotor::STEP_MODE::STEP_SIXTEEN);
	stepper->enable();
	stepper->wake();

	stepper->step(1);
}




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


//while (1){


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

//	cout << "Present Position: " + to_string(elbowP - 10) << endl;


	// Close device

//	printf( "Press Enter key to terminate...\n" );
//	getchar();

//	int GoalPos[2] = {0, 1023};
//	dxl_write_word( ARM_ELBOW2_ID, P_GOAL_POSITION_L, GoalPos[elbowP + 10] );

	dxl_terminate();
}
