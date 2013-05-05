// JausTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <openjaus.h>
#include "openjaus/core/Base.h"
#include "openjaus/mobility.h"
#include "openjaus/mobility/LocalPoseSensor.h"
#include "openjaus\mobility\VelocityStateSensor.h"
#include "MyComponent.h"
using namespace openjaus;
using namespace std;
using namespace mobility;


MyComponent component(1, 1, 2);

void printMenu()
{
	std::cout << "Menu:" << std::endl;
	std::cout << "t - Print System Tree" << std::endl;
	std::cout << "1 - Control Status" << std::endl;
	std::cout << "2 - Print ReportLocalPose" << std::endl;
	std::cout << "3 - Print ReportVelocityState" << std::endl;
	
	std::cout << "? - Output Menu" << std::endl;
	std::cout << "ESC - Exit Component" << std::endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	openjaus::system::Application::setTerminalMode();
	
	component.run();
	printMenu();

	unsigned char choice = 0;
	while (choice != 27 && choice != 'q')
	{
		choice = openjaus::system::Application::getChar();
		switch(choice)
		{
		case 't':
			std::cout << component.getSystemTree()->toString();
			break;

		case '?':
			printMenu();
			break;

		case '1':
			cout << "Not implemented\n";
			//TODO: get controller address and display
			break;

		case '2':
			printf("ReportLocalPose:\tX: %f\tY: %f\tYaw: %f\n", 
			component.reportLocalPose.getX_m(),
			component.reportLocalPose.getY_m(),
			component.reportLocalPose.getYaw_rad());
			break;

		case '3':
			printf("ReportVelocityState:\tVel_X: %f\tYaw_rate: %f\n", 
				component.reportVelocityState.getVelocityX_mps(),
				component.reportVelocityState.getYawRate_rps());
			break;
		}

	}
	return 0;
}

