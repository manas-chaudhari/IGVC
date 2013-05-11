// COP_Simulator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <openjaus.h>
#include <openjaus\core.h>
#include <openjaus\core\Base.h>
#include <openjaus/mobility.h>

#define DEBUG 1
#define APPLICATION_DISCOVERY 1
#define DELAY 4
using namespace openjaus;
using namespace std;

core::Base component;
int nQueryIdentification = 0;
int controlStage = 0;
bool queryStatusEnabled = false;
//system::Timer *queryTimer;
void sendQueryStatus(void* data, system::Timer *timer);
system::Timer queryTimer(sendQueryStatus, NULL);

transport::Address machineAddress(1,1,1);

void runTest();
void processCommand(unsigned char choice);

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

bool processQueryIdentification(core::QueryIdentification &queryIdentification)
{
	printf("Recv QueryIdentification count: %d\n", nQueryIdentification);
	machineAddress = queryIdentification.getSource();
	
	if (nQueryIdentification >= DELAY)
	{
		core::ReportIdentification *reportIdentification = new core::ReportIdentification();
		reportIdentification->setDestination(machineAddress);
		component.sendMessage(reportIdentification);
		printf("ReportIdentification sent");
	}
	
	nQueryIdentification++;
	return true;
}

bool processReportServices(core::ReportServices &reportServices)
{
	core::ServicesNodeList nodeList = reportServices.getRSNodeList();
	core::ServicesComponentList cmpList = nodeList.get(0).getServicesComponentList();
	for(size_t i = 0; i < cmpList.size(); i++)
	{
		core::ServicesComponentRecord cmp = cmpList.get(i);
		if (cmp.getComponentID() == machineAddress.getComponent())
		{
			core::ServicesServiceList services = cmp.getServicesServiceList();
			std::cout << "Service list:\n";
			for (size_t j = 0; j < services.size(); j++)
				std::cout << services.get(j).getUri() << std::endl;
			cout << "\n";
			break;
		}
	}
	return false;
}

bool processReportControl(core::ReportControl &reportControl)
{
	transport::Address copAddress = component.getAddress();
	//if(reportControl.getSubsystemID() == copAddress.getSubsystem(), reportControl.getNodeID() == copAddress.getNode(), reportControl.getComponentID() == copAddress.getComponent())
	if(reportControl.getSubsystemID() || reportControl.getNodeID() || reportControl.getComponentID())
		cout << "Received ReportControl: Controlled\n";
	else
	{
		cout << "Received ReportControl: NotControlled\n";
		controlStage = 1;
	}
	return true;
}

bool processConfirmControl(core::ConfirmControl &confirmControl)
{
	cout << "Received ConfirmControl: "; 
	switch(confirmControl.getConfirmResponseCode())
	{
	case core::ConfirmResponseCodeEnumeration::CONTROL_ACCEPTED:
		cout << "Control Accepted\n";
		break;

	default:
		cout << "Control rejected\n";
		break;
	}
	return false;
}

bool processReportStatus(core::ReportStatus &reportStatus)
{
	cout << "Recvd ReportStatus: ";
	switch(reportStatus.getStatus())
	{
	case core::StatusEnumeration::EMERGENCY:
		cout << "EMERGENCY";
		break;

	case core::StatusEnumeration::READY:
		cout << "READY";
		break;
		
	case core::StatusEnumeration::INIT:
		cout << "INIT";
		break;

	case core::StatusEnumeration::SHUTDOWN:
		cout << "SHUTDOWN";
		break;
		
	case core::StatusEnumeration::STANDBY:
		cout << "STANDBY";
		break;

	case core::StatusEnumeration::FAILURE:
		cout << "FAILURE";
		break;

	}
	cout << "\n";
	return true;
}

void printMenu()
{
	std::cout << "Menu:" << std::endl;
	std::cout << "t - Print System Tree" << std::endl;
	
	std::cout << "1 - Query Capabilities" << std::endl;
	std::cout << "2 - Acquire Control" << std::endl;
	std::cout << "3 - Send periodic QueryStatus" << std::endl;
	
	std::cout << "4 - Query Local Pose" << std::endl;
	std::cout << "5 - Set Local Pose" << std::endl;

	std::cout << "6 - Query Velocity State" << std::endl;

	std::cout << "7 - Standby" << std::endl;
	std::cout << "8 - Resume" << std::endl;
	std::cout << "9 - Shutdown" << std::endl;
	
	std::cout << "s - Set Destination Address" << std::endl;
	std::cout << "l - Find LocalPoseSensor" << std::endl;
	std::cout << "v - Find VelocityStateSensor" << std::endl;
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

int _tmain(int argc, _TCHAR* argv[])
{
	openjaus::system::Application::setTerminalMode();
	Sleep(5000);

	component.setName("COP");
	component.addMessageCallback(processReportLocalPose);
	component.addMessageCallback(processReportVelocityState);

	component.addMessageCallback(processQueryIdentification);

	component.addMessageCallback(processReportServices);
	component.addMessageCallback(processReportControl);
	component.addMessageCallback(processConfirmControl);
	component.addMessageCallback(processReportStatus);

	component.run();
	
	Sleep(1000);

#if DEBUG

#if APPLICATION_DISCOVERY
	while (nQueryIdentification < DELAY + 1)
	{
		
	}
	printf("\n\nDiscovery Complete. Machine Address: %d %d %d\n\n", machineAddress.getSubsystem(), machineAddress.getNode(), machineAddress.getComponent());

#else
	std::vector<transport::Address> componentList;
	componentList = component.getSystemTree()->lookupService("urn:jaus:jss:mobility:LocalPoseSensor");

	if (componentList.size() > 0)
		machineAddress = componentList.at(0);
	else
		machineAddress = readDestinationAddress();

#endif

	queryTimer.setInterval(5000);

	printMenu();

	unsigned char choice = 0;
	while (choice != 27)
	{
		choice = system::Application::getChar();
		processCommand(choice);
	}
#else
	
	runTest();
	
#endif
	//component.stop();
	
	return 0;
}

void runTest()
{
	//Todo: Auto Test all requirements

}

void sendQueryControl()
{
	core::QueryControl *queryControl = new core::QueryControl();
	queryControl->setDestination(machineAddress);
	component.sendMessage(queryControl);

	cout << "Sent QueryControl\n";
}

void sendQueryStatus(void* data, system::Timer *timer)
{
	if (!queryStatusEnabled)
		return;
	core::QueryStatus *queryStatus = new core::QueryStatus();
	queryStatus->setDestination(machineAddress);
	component.sendMessage(queryStatus);

	cout << "Sent QueryStatus\n";
}

void processCommand(unsigned char choice)
{
	std::vector<transport::Address> componentList;

	switch(choice)
	{
	case 't':
		std::cout << component.getSystemTree()->toString();
		break;

	case '?':
		printMenu();
		break;

	case 's':
		/*std::cout << "Enter destination name: ";
		char destname[20];
		std::cin >> destname;
		componentList = component.getSystemTree()->lookupComponent(destname);
		std::cout << "Found " << componentList.size() << " components.\n";
		break;*/

		machineAddress = readDestinationAddress();
		break;

	case '1':
		std::cout << "Sending QueryServices" << std::endl;
		{
			core::QueryServices *query = new core::QueryServices();

			query->setDestination(machineAddress);
			component.sendMessage(query);
		}
		break;

	case '2':
		std::cout << "Acquiring Control" << std::endl;
		sendQueryControl();
		while(controlStage < 2)
		{
			if (controlStage == 0)
			{
				system::Time::sleep(5000);
				sendQueryControl();
			}
			else
			{
				core::RequestControl *requestControl = new core::RequestControl();
				requestControl->setDestination(machineAddress);
				requestControl->setAuthorityCode(component.getAuthority());
				component.sendMessage(requestControl);
				cout << "Sent RequestControl\n";
				break;
			}
		}
		break;

	case '3':
		std::cout << "Enabling periodic QueryStatus" << std::endl;
		queryStatusEnabled = true;
		break;

	case '4':
		std::cout << "Sent QueryLocalPose" << std::endl;
		{
			mobility::QueryLocalPose *query = new mobility::QueryLocalPose();
			
			query->setQueryPresenceVector(65535);
			query->setDestination(machineAddress);
			component.sendMessage(query);
		}
		break;

	case '5':
		std::cout << "Sent SetLocalPose" << std::endl;
		{
			mobility::SetLocalPose *query = new mobility::SetLocalPose();
				
			std::cout << "Enter X, Y, Yaw: " << "\n";
			double x, y, yaw;
			std::cin >> x >> y >> yaw;
			query->setPresenceVector(65535);
			query->setX_m(x);
			query->setY_m(y);
			query->setYaw_rad(yaw);

			query->setDestination(machineAddress);
				
			component.sendMessage(query);
		}
		break;

	case '6':
		std::cout << "Sending Query VelocityState" << std::endl;
		{
			mobility::QueryVelocityState *query = new mobility::QueryVelocityState();
				
			query->setQueryPresenceVector(65535);"\n";
			query->setDestination(machineAddress);
			component.sendMessage(query);
		}
		break;
		
	case '7':
		std::cout << "Sending Standby" << std::endl;
		{
			core::Standby *message = new core::Standby();
				
			message->setDestination(machineAddress);
			component.sendMessage(message);
		}
		break;
			
	case '8':
		std::cout << "Sending Resume" << std::endl;
		{
			core::Resume *message = new core::Resume();
				
			message->setDestination(machineAddress);
			component.sendMessage(message);
		}
		break;
			
	case '9':
		std::cout << "Sending Shutdown" << std::endl;
		{
			core::Shutdown *message = new core::Shutdown();
				
			message->setDestination(machineAddress);
			component.sendMessage(message);
		}
		break;
		

	case 'l':
		componentList = component.getSystemTree()->lookupService("urn:jaus:jss:mobility:LocalPoseSensor");
		std::cout << "Local Pose Services (" << componentList.size() << "):\n";
		for(size_t i = 0; i < componentList.size(); i++)
			{
				std::cout << "\t" << componentList.at(i).toString() << std::endl;
			}
			break;
		break;
		
	case 'v':
		componentList = component.getSystemTree()->lookupService("urn:jaus:jss:mobility:VelocityStateSensor");
		std::cout << "VelocityState Services (" << componentList.size() << "):\n";
		for(size_t i = 0; i < componentList.size(); i++)
			{
				std::cout << "\t" << componentList.at(i).toString() << std::endl;
			}
			break;
		break;

	/*case '2':
		std::cout << "Sending Query Identification" << std::endl;
		{
			core::QueryIdentification *query = new core::QueryIdentification();
			query->setName(component.getName());
			query->setId(4);
			query->setDestination(machineAddress);
			component.sendMessage(query); 
		}

		break;*/

	}

}
