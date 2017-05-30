//  ---------------------- Doxygen info ----------------------
//! \file LWRJointImpedanceControlExample.cpp
//!
//! \brief
//! Sample application for the class LWRJointImpedanceController
//!
//! \details
//! This simple application feature a sample of how to use the
//! joint impedance controller of the KUKA Fast Research Interface
//! for the Light-Weight Robot IV. For details about the actual
//! interface class (i.e., class LWRJointImpedanceController), please
//! refer to the file LWRJointImpedanceController.h.
//!
//! \date December 2014
//!
//! \version 1.2
//!
//!	\author Torsten Kroeger, tkr@stanford.edu\n
//! \n
//! Stanford University\n
//! Department of Computer Science\n
//! Artificial Intelligence Laboratory\n
//! Gates Computer Science Building 1A\n
//! 353 Serra Mall\n
//! Stanford, CA 94305-9010\n
//! USA\n
//! \n
//! http://cs.stanford.edu/groups/manips\n
//! \n
//! \n
//! \copyright Copyright 2014 Stanford University\n
//! \n
//! Licensed under the Apache License, Version 2.0 (the "License");\n
//! you may not use this file except in compliance with the License.\n
//! You may obtain a copy of the License at\n
//! \n
//! http://www.apache.org/licenses/LICENSE-2.0\n
//! \n
//! Unless required by applicable law or agreed to in writing, software\n
//! distributed under the License is distributed on an "AS IS" BASIS,\n
//! WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n
//! See the License for the specific language governing permissions and\n
//! limitations under the License.\n
//! 
//  ----------------------------------------------------------
//   For a convenient reading of this file's source code,
//   please use a tab width of four characters.
//  ----------------------------------------------------------


#include <LWRJointImpedanceController.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <ctype.h> // need for toupper


#ifndef NUMBER_OF_JOINTS
#define NUMBER_OF_JOINTS			7
#endif

#define RUN_TIME_IN_SECONDS		60.0

#define RIGHT_INIT_FILE "/home/ifma/projects/ros_sigma_platform_fri_ws/src/single_lwr_robot/config/49938-FRI-Driver.init"
#define LEFT_INIT_FILE "/home/ifma/projects/ros_sigma_platform_fri_ws/src/single_lwr_robot/config/49939-FRI-Driver.init"


char* robotType;
char *robotTypeUpper;

//*******************************************************************************************
// main()

char * strtoupper( char * dest, const char * src ) {
    char * result = dest;
    while( *dest++ = toupper( *src++ ) );
    return result;
}

int main(int argc, char *argv[])
{

	if (argc > 1)
	{
		robotType = strdup(argv[1]);
		robotTypeUpper = strdup(argv[1]);
		strtoupper(robotTypeUpper,robotType);
	}
	else
	{
		printf("\nUsage = ./exe left|right\n");
		return -1;
	}

	printf("\nYou choose to work with the %s arm\n",robotTypeUpper);

	unsigned int				CycleCounter	=	0
							,	i				=	0;

	int							ResultValue		=	0;

	float						CommandedTorquesInNm	[NUMBER_OF_JOINTS]
		 					,	CommandedStiffness		[NUMBER_OF_JOINTS]
		 					,	CommandedDamping		[NUMBER_OF_JOINTS]
		 					,	MeasuredTorquesInNm		[NUMBER_OF_JOINTS]
		 					,	JointValuesInRad		[NUMBER_OF_JOINTS];

	LWRJointImpedanceController	*Robot;

	if (strcmp(robotTypeUpper,"RIGHT") == 0)
		Robot	=	new LWRJointImpedanceController(RIGHT_INIT_FILE);
	else
		Robot	=	new LWRJointImpedanceController(LEFT_INIT_FILE);

	fprintf(stdout, "RobotJointImpedanceController object created. Starting the robot...\n");

	for (i = 0; i < NUMBER_OF_JOINTS; i++)
	{
		CommandedStiffness	[i]	=	(float) 0.0;//(float)200.0;
		CommandedDamping	[i]	=	(float) 0.0;//(float)0.7;
		CommandedTorquesInNm[i]	=	(float)0.0;
	}

	Robot->SetCommandedJointStiffness	(CommandedStiffness		);
	Robot->SetCommandedJointDamping		(CommandedDamping		);
	Robot->SetCommandedJointTorques		(CommandedTorquesInNm	);

	ResultValue	=	Robot->StartRobot();

	if (ResultValue == EOK)
	{
		fprintf(stdout, "Robot successfully started.\n");
	}
	else
	{
		fprintf(stderr, "ERROR, could not start robot: %s\n", strerror(ResultValue));
	}

	fprintf(stdout, "Current system state:\n%s\n", Robot->GetCompleteRobotStateAndInformation());

	Robot->GetMeasuredJointPositions(JointValuesInRad);

	fprintf(stdout, "Performing joint impedance control for %.1f seconds.\n", RUN_TIME_IN_SECONDS);

	while ((float)CycleCounter * Robot->GetCycleTime() < RUN_TIME_IN_SECONDS)
	{
		Robot->WaitForKRCTick();

		if (!Robot->IsMachineOK())
		{
			fprintf(stderr, "ERROR, the machine is not ready anymore.\n");
			break;
		}

		Robot->GetMeasuredJointTorques		(MeasuredTorquesInNm	);
		Robot->GetMeasuredJointPositions	(JointValuesInRad		);
		fprintf(stdout,"\n");
		fflush(stdout);
		for (int i=0; i < NUMBER_OF_JOINTS; i++)
		{
			fprintf(stdout,"J[%d]=%f;",JointValuesInRad[i]);
			fflush(stdout);
		}
		fprintf(stdout,"\n");
		fflush(stdout);
		Robot->SetCommandedJointPositions	(JointValuesInRad);
		Robot->SetCommandedJointStiffness	(CommandedStiffness		);
		Robot->SetCommandedJointDamping		(CommandedDamping		);
		Robot->SetCommandedJointTorques		(CommandedTorquesInNm	);

		CycleCounter++;
	}

	fprintf(stdout, "Stopping the robot...\n");
	ResultValue	=	Robot->StopRobot();

	if (ResultValue != EOK)
	{
		fprintf(stderr, "An error occurred during stopping the robot... ResultValue = %d\n", ResultValue);
	}
	else
	{
		fprintf(stdout, "Robot successfully stopped.\n");
	}

	fprintf(stdout, "Deleting the object...\n");
	delete Robot;
	fprintf(stdout, "Object deleted...\n");

	return(EXIT_SUCCESS);
}
