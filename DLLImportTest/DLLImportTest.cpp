// DLLImportTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../IGVCJausDLL/IGVCJausDLL.h"
#include <iostream>
using namespace std;

void printMenu()
{
	std::cout << "Menu:" << std::endl;
	std::cout << "1 - Number of Messages in queue" << std::endl;
	std::cout << "2 - Simulate Message" << std::endl;
	std::cout << "3 - Update Local Pose (50, 10, 1.2), VelocityState(4, 0.6)" << std::endl;
	std::cout << "4 - Read Message" << std::endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	init(1,1,2);

	printMenu();

	unsigned char choice = 0;
	while (choice != 'q')
	{
		choice = getchar();
		switch(choice)
		{
		case '?':
			printMenu();
			break;

		case '1':
			printf("Messages: %d\n", loop(0));
			break;
			
		case '2':
			//simulate_message_received();
			break;

		case '3':
			updateLocalPose(50, 10, 1.2);
			updateVelocityState(4, 0.6);
			break;

		case '4':
			char message[200];
			readMessage(message);
			cout << "Message: \n" << message << endl;
		}
	}
	
	//stop();
	return 0;
}

