#include <FastResearchInterface.h>
#include <TypeIRML.h>
#include <OSAbstraction.h> // need for WaitForKBCharacter
#include <iostream>
#include <string>
#include <algorithm>
#include <math.h>
#include <cstring> // need for memset

#ifndef PI
	#define PI 3.1415926535897932384626433832795
#endif

#ifndef RAD
	#define RAD(A)	((A) * PI / 180.0 )
#endif

#ifndef DEG
	#define DEG(A)	((A) * 180.0 / PI )
#endif

#ifndef NUMBER_OF_JOINTS
	#define NUMBER_OF_JOINTS 7
#endif

#define RIGHT_INIT_FILE "/home/ifma/git_project/FRI_IP/etc/49938-FRI-Driver.init"
#define LEFT_INIT_FILE "/home/ifma/git_project/FRI_IP/etc/49939-FRI-Driver.init"

using namespace std;

string robotType;

void raz(float *tab, const int nb)
{
	memset(tab,0x0,nb*sizeof(float));
}


void moveWithSin(FastResearchInterface *FRI)
{

	float		FunctionValue	=	0.0
		,	LoopVariable	=	0.0
		,	JointValuesInRad[NUMBER_OF_JOINTS]
                ,	InitialJointValuesInRad[NUMBER_OF_JOINTS];

	int i;

       FRI->GetMeasuredJointPositions(InitialJointValuesInRad);


	while (LoopVariable < 5.0 * PI)
	{
		FRI->WaitForKRCTick();

		if (!FRI->IsMachineOK())
		{
			fprintf(stderr, "ERROR, the machine is not ready anymore.\n");
			break;
		}

		FunctionValue	=	(float)(0.3 * sin(LoopVariable));
		FunctionValue	*=	(float)FunctionValue;

		for (i = 0; i < NUMBER_OF_JOINTS; i++)
		{
			JointValuesInRad[i]	=	InitialJointValuesInRad[i] + FunctionValue;
		}

		FRI->SetCommandedJointPositions(JointValuesInRad);

		LoopVariable	+=	(float)0.001;
	}

}

void moveToPosition(FastResearchInterface *FRI, TypeIRML *RML, TypeIRMLInputParameters *IP, TypeIRMLOutputParameters *OP)
{
	int resultValue = TypeIRML::RML_WORKING;
	
	float jointValuesInRad[NUMBER_OF_JOINTS];


    while ((FRI->IsMachineOK()) && (resultValue != TypeIRML::RML_FINAL_STATE_REACHED))
    {
        FRI->WaitForKRCTick();

        resultValue	=	RML->GetNextMotionState_Position(*IP,OP);


        if ((resultValue != TypeIRML::RML_WORKING) && (resultValue != TypeIRML::RML_FINAL_STATE_REACHED))
        {
            cout << "MoveToReasonableStartPosition(): ERROR during trajectory generation " << resultValue << endl;
        }

        for ( int i = 0; i < NUMBER_OF_JOINTS; i++)
        {
            jointValuesInRad[i] = RAD((double)(OP->NewPosition->VecData[i]));
        }


        FRI->SetCommandedJointPositions(jointValuesInRad);

        *(IP->CurrentPosition) = *(OP->NewPosition);
        *(IP->CurrentVelocity) = *(OP->NewVelocity);

    }
	
}


int main(int argc, char *argv[])
{

	if (argc > 1)
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
	int resultValue	= 0;
	char c;

	if (robotType.compare("RIGHT") == 0)
		FRI = new FastResearchInterface(RIGHT_INIT_FILE);
	else
		FRI = new FastResearchInterface(LEFT_INIT_FILE);

	cout << "\nRobotJointImpedanceController object created..." << endl;
	
	cout << "\nPlease press any key..." << endl;

	c = WaitForKBCharacter(NULL);

	unsigned int controlScheme = FastResearchInterface::JOINT_POSITION_CONTROL;

	cout << "Control strategy set to joint position control." << endl;

	cout << "Start Robot !" << endl;

	resultValue = FRI->StartRobot(controlScheme);
	
	if (resultValue != EOK)
	{
		cout << "\nAn error occurred during starting up the robot..." << endl;
	}

	cout << "Result of Start Robot = " << resultValue << endl;
	
    
	
    float jointValuesInRad[NUMBER_OF_JOINTS];

    double cycleTime = 0.002;

    TypeIRML *RML = NULL;

    TypeIRMLInputParameters *IP = NULL;

    TypeIRMLOutputParameters *OP=NULL;

    RML = new TypeIRML(NUMBER_OF_JOINTS,cycleTime);

    IP = new TypeIRMLInputParameters(NUMBER_OF_JOINTS);

    OP = new TypeIRMLOutputParameters(NUMBER_OF_JOINTS);

    raz(jointValuesInRad,NUMBER_OF_JOINTS);
    
    cout << "Moving to the a reasonable start position.." << endl;

    double targetJointPosition[7];
    
    for (int j=0; j<4; ++j)
    {
   
		FRI->GetMeasuredJointPositions(jointValuesInRad);
		 
		targetJointPosition[0] = 0.0;
		targetJointPosition[1] = 27;
		targetJointPosition[2] = 0.0;
		targetJointPosition[3] = -100;
		targetJointPosition[4] = 0.0;
		targetJointPosition[5] = -27.5;
		targetJointPosition[6] = 0.0;
		

		for ( int i = 0; i < NUMBER_OF_JOINTS; i++)
		{
			IP->CurrentPosition->VecData[i] = (double)DEG(jointValuesInRad[i]);
			IP->TargetPosition->VecData[i]	= (double)targetJointPosition[i];
			IP->MaxVelocity->VecData[i] = (double)50.0;
			IP->MaxAcceleration->VecData[i] = (double)20.0;
			IP->SelectionVector->VecData[i] = true;
		}

		moveToPosition(FRI, RML, IP, OP);
		
		FRI->GetMeasuredJointPositions(jointValuesInRad);
		
		targetJointPosition[0] = 0.0;
		targetJointPosition[1] = 1.55;
		targetJointPosition[2] = 0.0;
		targetJointPosition[3] = 0.0;
		targetJointPosition[4] = 0.0;
		targetJointPosition[5] = 0.0;
		targetJointPosition[6] = 0.0;
		
		for ( int i = 0; i < NUMBER_OF_JOINTS; i++)
		{
			IP->CurrentPosition->VecData[i] = (double)DEG(jointValuesInRad[i]);
			IP->TargetPosition->VecData[i]	= (double)DEG(targetJointPosition[i]);
			IP->MaxVelocity->VecData[i] = (double)75.0;
			IP->MaxAcceleration->VecData[i] = (double)50.0;
			IP->SelectionVector->VecData[i] = true;
		}
		
		moveToPosition(FRI, RML, IP, OP);
    }


    moveWithSin(FRI);
    
    if (!FRI->IsMachineOK())
    {
        cout << "MoveToReasonableStartPosition(): ERROR, machine is not ready." << endl;
    }
    
	resultValue	= FRI->StopRobot();	

	if (resultValue != EOK)
	{
		cout << "\nAn error occurred during stopping the robot..." << endl;
	}
   
    delete	RML;
    delete	IP;
    delete	OP;

  
	
	cout << "\nPlease press any key..." << endl;

	c = WaitForKBCharacter(NULL);
	
	cout << "\nDeleting the object..." << endl;
	
	delete FRI;
	
	cout << "\nObject deleted..." << endl;
	
}
