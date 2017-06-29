#include <FastResearchInterface.h>
//#include <OSAbstraction.h> // need for WaitForKBCharacter
#include <iostream>
#include <string>
#include <algorithm>
#include <math.h>
#include <cstring> // need for memset

//#include <unistd.h>


#ifndef NUMBER_OF_JOINTS
	#define NUMBER_OF_JOINTS 7
#endif

#ifndef PI
	#define PI	3.1415926535897932384626433832795
#endif

#define RIGHT_INIT_FILE "/home/ifma/projects/ros_sigma_platform_fri_ws/src/single_lwr_robot/config/49938-FRI-Driver.init"
#define LEFT_INIT_FILE "/home/ifma/projects/ros_sigma_platform_fri_ws/src/single_lwr_robot/config/49939-FRI-Driver.init"

#define RUN_TIME_IN_SECONDS 5.0

using namespace std;

string robotType;

void raz(float *tab, const int nb)
{
	memset(tab,0x0,nb*sizeof(float));
}

int main(int argc, char *argv[])
{

	/*if (argc > 1)
	{
		robotType = string(argv[1]);
		std::transform(robotType.begin(), robotType.end(),robotType.begin(), ::toupper);
	}
	else
	{
		cout << "\nUsage = ./exe left|right" << endl;
		return -1;
	}

	cout << "\nYou choose to work with the " << robotType << " arm\n" << endl;

	FastResearchInterface	*FRI;

	if (robotType.compare("RIGHT") == 0)
		FRI = new FastResearchInterface(RIGHT_INIT_FILE);
	else
		FRI = new FastResearchInterface(LEFT_INIT_FILE);

	cout << "\nRobotJointImpedanceController object created..." << endl;

	float   jointValuesInRad[NUMBER_OF_JOINTS],
			initialJointValuesInRad[NUMBER_OF_JOINTS];
			
	float	measuredTorquesInNm	[NUMBER_OF_JOINTS],
			commandedTorquesInNm [NUMBER_OF_JOINTS],
		 	commandedStiffness [NUMBER_OF_JOINTS],
		 	commandedDamping [NUMBER_OF_JOINTS];


	raz(jointValuesInRad,NUMBER_OF_JOINTS);
	raz(initialJointValuesInRad,NUMBER_OF_JOINTS);
	
	raz(measuredTorquesInNm,NUMBER_OF_JOINTS);
	raz(commandedTorquesInNm,NUMBER_OF_JOINTS);
	raz(commandedStiffness,NUMBER_OF_JOINTS);
	raz(commandedDamping,NUMBER_OF_JOINTS);

*/

	/*for (size_t i = 0; i < NUMBER_OF_JOINTS; i++)
	{
		jointStiffnessValues[i] = (float)10.0;
		jointDampingValues[i] = (float)0.7;
	}

	FRI->SetCommandedJointDamping(jointDampingValues);
	FRI->SetCommandedJointStiffness(jointStiffnessValues);
	*/
	/*
	int resultValue	= 0;
	char c;
	
	cout << "\nPlease press any key..." << endl;

	//c = WaitForKBCharacter(NULL);

	unsigned int controlScheme = FastResearchInterface::JOINT_POSITION_CONTROL;

	cout << "Control strategy set to joint position control." << endl;

	cout << "Start Robot !" << endl;

	resultValue = FRI->StartRobot(controlScheme,120.0);
	
	if (resultValue != EOK)
	{
		cout << "\nAn error occurred during starting up the robot..." << endl;
	}

	cout << "Result of Start Robot = " << resultValue << endl;
	
	
	cout << "Current system state:\n%s\n" << FRI->GetCompleteRobotStateAndInformation() << endl;
	
	if (FRI->IsMachineOK())
	{
		cout << "Machine is OK !" << endl;
	}
	else
	{
		cout << "Machine is not OK!" << endl;
	}
	
	float loopVariable	=	0.0;
	float functionValue = 0.0;
	
	FRI->GetMeasuredJointPositions(initialJointValuesInRad);
	
	while (loopVariable < 5.0 * PI)
	{
		FRI->WaitForKRCTick(0);

		if (!FRI->IsMachineOK())
		{
			cout << "\nERROR, the machine is not ready anymore" << endl;
			break;
		}

		functionValue	=	(float)(0.3 * sin(loopVariable));
		functionValue	*=	(float)functionValue;

		for (int i = 0; i < NUMBER_OF_JOINTS; i++)
		{
			jointValuesInRad[i]	=	initialJointValuesInRad[i] + functionValue;
		}

		FRI->SetCommandedJointPositions(jointValuesInRad);

		loopVariable	+=	(float)0.001;
	}
	
	cout << "\nStopping the FRI..." << endl;
	
	resultValue	= FRI->StopRobot();	

	if (resultValue != EOK)
	{
		cout << "\nAn error occurred during stopping the robot..." << endl;
	}
	
	cout << "\nPlease press any key..." << endl;

	//c = WaitForKBCharacter(NULL);
	
	controlScheme = FastResearchInterface::JOINT_IMPEDANCE_CONTROL;
	
	cout << "Control strategy set to joint impedance control." << endl;
	
	
	for (int i = 0; i < NUMBER_OF_JOINTS; i++)
	{
		commandedStiffness	[i]	=	(float)200.0;
		commandedDamping	[i]	=	(float)0.7;
		commandedTorquesInNm[i]	=	(float)1.0;
	}

	FRI->SetCommandedJointStiffness(commandedStiffness);
	FRI->SetCommandedJointDamping(commandedDamping);
	FRI->SetCommandedJointTorques(commandedTorquesInNm);

	cout << "Start Robot !" << endl;

	resultValue = FRI->StartRobot(controlScheme,120.0);
	
	if (resultValue != EOK)
	{
		cout << "\nAn error occurred during starting up the robot..." << endl;
	}

	cout << "Result of Start Robot = " << resultValue << endl;
	
	unsigned int cycleCounter =	0;
	
	FRI->GetMeasuredJointPositions(jointValuesInRad);

	cout << "Performing joint impedance control for " <<  RUN_TIME_IN_SECONDS << " seconds." << endl;

	while ((float)cycleCounter * FRI->GetFRICycleTime() < RUN_TIME_IN_SECONDS)
	{
		FRI->WaitForKRCTick(0);

		if (!FRI->IsMachineOK())
		{
			cout << "ERROR, the machine is not ready anymore." << endl;
			break;
		}

		FRI->GetMeasuredJointTorques(measuredTorquesInNm);
		FRI->GetMeasuredJointPositions(jointValuesInRad);
		
		FRI->SetCommandedJointPositions(jointValuesInRad);
		FRI->SetCommandedJointStiffness(commandedStiffness);
		FRI->SetCommandedJointDamping(commandedDamping);
		FRI->SetCommandedJointTorques(commandedTorquesInNm);

		cycleCounter++;
	}
	
	cout << "\nStopping the FRI..." << endl;
	
	resultValue	= FRI->StopRobot();	

	if (resultValue != EOK)
	{
		cout << "\nAn error occurred during stopping the robot..." << endl;
	}
	
	cout << "\nPlease press any key..." << endl;

	//c = WaitForKBCharacter(NULL);
	
	controlScheme = FastResearchInterface::JOINT_POSITION_CONTROL;

	cout << "Control strategy set to joint position control." << endl;

	cout << "Start Robot !" << endl;

	resultValue = FRI->StartRobot(controlScheme,120.0);
	
	if (resultValue != EOK)
	{
		cout << "\nAn error occurred during starting up the robot..." << endl;
	}

	cout << "Result of Start Robot = " << resultValue << endl;
	
	
	if (FRI->IsMachineOK())
	{
		cout << "Machine is OK !" << endl;
	}
	else
	{
		cout << "Machine is not OK!" << endl;
	}
	
	loopVariable	=	0.0;
	functionValue = 0.0;
	
	FRI->GetMeasuredJointPositions(initialJointValuesInRad);
	
	while (loopVariable < 5.0 * PI)
	{
		FRI->WaitForKRCTick(0);

		if (!FRI->IsMachineOK())
		{
			cout << "\nERROR, the machine is not ready anymore" << endl;
			break;
		}

		functionValue	=	(float)(0.3 * sin(loopVariable));
		functionValue	*=	(float)functionValue;

		for (int i = 0; i < NUMBER_OF_JOINTS; i++)
		{
			jointValuesInRad[i]	=	initialJointValuesInRad[i] + functionValue;
		}

		FRI->SetCommandedJointPositions(jointValuesInRad);

		loopVariable	+=	(float)0.001;
	}
	
	cout << "\nStopping the FRI..." << endl;
	
	resultValue	= FRI->StopRobot();	

	if (resultValue != EOK)
	{
		cout << "\nAn error occurred during stopping the robot..." << endl;
	}
	
	sleep(2);
	
	cout << "\nDeleting the object..." << endl;
	
	delete FRI;*/
	
	cout << "\nObject deleted..." << endl;
	

}


