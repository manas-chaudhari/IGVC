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

void printMenu()
{
	std::cout << "Menu:" << std::endl;
	std::cout << "t - Print System Tree" << std::endl;
	std::cout << "1 - Control Status" << std::endl;
	std::cout << "2 - Print ReportLocalPose" << std::endl;
	/*std::cout << "3 - Find LocalPoseSensor" << std::endl;
	std::cout << "4 - Query Local Pose" << std::endl;
	std::cout << "5 - Find VelocitySensor" << std::endl;
	std::cout << "6 - Query Velocity State" << std::endl;
	std::cout << "7 - Query Services" << std::endl;*/
	/*std::cout << "4 - Create Periodic Event (Report GPOS)" << std::endl;
	std::cout << "5 - Stop Periodic Event (Report GPOS)" << std::endl;
	std::cout << "6 - Request GPOS Control" << std::endl;
	std::cout << "7 - Release GPOS Control" << std::endl;
	std::cout << "8 - Set Global Pose" << std::endl;*/
	std::cout << "? - Output Menu" << std::endl;
	std::cout << "ESC - Exit Component" << std::endl;
}

bool processSetLocalPose(mobility::SetLocalPose &message)
{
	cout << "x: " << message.getX_m() << "\n";
	cout << "id: " << message.getId() << "\n";
	cout << "name: " << message.getName() << "\n";
	cout << "MId: " << message.getMessageID() << "\n";
	return true;
}


int _tmain(int argc, _TCHAR* argv[])
{
	openjaus::system::Application::setTerminalMode();

	MyComponent component;
	//component.addMessageCallback(&processSetLocalPose);

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

		}

	}
	return 0;
}

