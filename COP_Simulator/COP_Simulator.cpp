// COP_Simulator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <openjaus.h>
#include <openjaus\core.h>
#include <openjaus\core\Base.h>
#include <openjaus/mobility.h>

using namespace openjaus;

bool processReportLocalPose(mobility::ReportLocalPose &reportLocalPose)
{
	/*std::cout << "Recv Report Local Pose\n";
	std::cout << "Latitude: " << report.getX_m() << std::endl;
	std::cout << "Longitude: " << report.getY_m() << std::endl;
	std::cout << "Yaw: " << report.getYaw_rad() << std::endl << std::endl;
*/
	printf("Recvd ReportLocalPose:\tX: %f\tY: %f\tYaw: %f\n\n", 
				reportLocalPose.getX_m(),
				reportLocalPose.getY_m(),
				reportLocalPose.getYaw_rad());

	return true;
}

bool processReportVelocityState(mobility::ReportVelocityState &reportVelocityState)
{
	printf("Recvd ReportVelocityState:\tVel_X: %f\tYaw_rate: %f\n\n", 
				reportVelocityState.getVelocityX_mps(),
				reportVelocityState.getYawRate_rps());
	return true;
}

void processControlResponse(const model::ControlResponse& response)
{
	std::cout << "Recv Control Request Response from: " << response.getAddress() << std::endl;
	std::cout << "Response code: " << response.getResponseType() << std::endl;
}

void printMenu()
{
	std::cout << "Menu:" << std::endl;
	std::cout << "t - Print System Tree" << std::endl;
	std::cout << "1 - Set Destination Address" << std::endl;
	std::cout << "2 - Query Identification" << std::endl;
	std::cout << "3 - Find LocalPoseSensor" << std::endl;
	std::cout << "4 - Query Local Pose" << std::endl;
	std::cout << "5 - Request Control" << std::endl;
	std::cout << "6 - Set Local Pose" << std::endl;
	std::cout << "7 - Find VelocitySensor" << std::endl;
	std::cout << "8 - Query Velocity State" << std::endl;
	/*std::cout << "4 - Create Periodic Event (Report GPOS)" << std::endl;
	std::cout << "5 - Stop Periodic Event (Report GPOS)" << std::endl;
	std::cout << "6 - Request GPOS Control" << std::endl;
	std::cout << "7 - Release GPOS Control" << std::endl;
	std::cout << "8 - Set Global Pose" << std::endl;*/
	std::cout << "? - Output Menu" << std::endl;
	std::cout << "ESC - Exit Component" << std::endl;
}

transport::Address readDestinationAddress()
{
	std::cout << "Enter destination address: ";
	unsigned int _node, _subsystem, _component;
	std::cin >> _subsystem >> _node >> _component;
	return transport::Address(_subsystem, _node, _component);
}

core::Base component;

int _tmain(int argc, _TCHAR* argv[])
{
	openjaus::system::Application::setTerminalMode();
	Sleep(5000);
	transport::Address destinationAddress;// = readDestinationAddress();
	std::vector<transport::Address> componentList;

	component.setName("COP");
	component.addMessageCallback(processReportLocalPose);
	component.addMessageCallback(processReportVelocityState);

	//component.addMessageCallback(processReportServices);
	//component.addMessageCallback(processReportIdentification);
	//component.addMessageCallback(processQueryIdentification);
	component.run();
	
	Sleep(1000);
	componentList = component.getSystemTree()->lookupService("urn:jaus:jss:mobility:LocalPoseSensor");

	if (componentList.size() > 0)
		destinationAddress = componentList.at(0);
	else
		destinationAddress = readDestinationAddress();


	
	printMenu();

	unsigned char choice = 0;
	while (choice != 27)
	{
		choice = system::Application::getChar();
		switch(choice)
		{
		case 't':
			std::cout << component.getSystemTree()->toString();
			break;

		case '?':
			printMenu();
			break;

		case '1':
			/*std::cout << "Enter destination name: ";
			char destname[20];
			std::cin >> destname;
			componentList = component.getSystemTree()->lookupComponent(destname);
			std::cout << "Found " << componentList.size() << " components.\n";
			break;*/

			destinationAddress = readDestinationAddress();
			break;

		case '2':
			std::cout << "Sending Query Identification" << std::endl;
			{
				core::QueryIdentification *query = new core::QueryIdentification();
				query->setName(component.getName());
				query->setId(4);
				query->setDestination(destinationAddress);
				component.sendMessage(query); 
			}

			break;

		case '3':
			componentList = component.getSystemTree()->lookupService("urn:jaus:jss:mobility:LocalPoseSensor");
			std::cout << "Local Pose Services (" << componentList.size() << "):\n";
			for(size_t i = 0; i < componentList.size(); i++)
				{
					std::cout << "\t" << componentList.at(i).toString() << std::endl;
				}
				break;
			break;

		case '4':
			std::cout << "Sending Query Local Pose" << std::endl;
			{
				mobility::QueryLocalPose *query = new mobility::QueryLocalPose();
				//query->setName(component.getName());
				//query->setId(4);


				query->setQueryPresenceVector(65535);
				std::cout << "Destination Address: " << destinationAddress.toString() << "\n";
				query->setDestination(destinationAddress);
				component.sendMessage(query);
			}
			break;

		case '5':
			std::cout << "Requesting control" << std::endl;
			component.requestControl(destinationAddress, processControlResponse);
			break;

		case '6':
			std::cout << "Sending SetLocalPose" << std::endl;
			{
				mobility::SetLocalPose *query = new mobility::SetLocalPose();
				
				std::cout << "Enter X, Y, Yaw: " << "\n";
				double x, y, yaw;
				std::cin >> x >> y >> yaw;
				query->setPresenceVector(65535);
				query->setX_m(x);
				query->setY_m(y);
				query->setYaw_rad(yaw);

				query->setDestination(destinationAddress);
				
				component.sendMessage(query);
			}
			break;

		case '7':
			componentList = component.getSystemTree()->lookupService("urn:jaus:jss:mobility:VelocityStateSensor");
			std::cout << "VelocityState Services (" << componentList.size() << "):\n";
			for(size_t i = 0; i < componentList.size(); i++)
				{
					std::cout << "\t" << componentList.at(i).toString() << std::endl;
				}
				break;
			break;

		case '8':
			std::cout << "Sending Query VelocityState" << std::endl;
			{
				mobility::QueryVelocityState *query = new mobility::QueryVelocityState();
				
				query->setQueryPresenceVector(65535);"\n";
				query->setDestination(destinationAddress);
				component.sendMessage(query);
			}
			break;
		}
	}
	//component.stop();
	return 0;
}
