// IGVCJausDLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "IGVCJausDLL.h"
//#include <memory>

//#include <cstring>
#include <openjaus.h>
#include <openjaus\core.h>
#include <openjaus\core\Base.h>
#include <openjaus\mobility\LocalPoseSensor.h>
#include "messages.h"


using namespace openjaus;
using namespace std;

core::Base *component;
vector<model::Message*> messages;
//vector<std::tr1::shared_ptr<model::Message*>> messages;

/******** Messages *********/
mobility::ReportLocalPose *reportLocalPose;

bool processQueryServices(core::QueryServices &query)
{
	std::cout << "Recv Query Services from: " << query.getSource() << std::endl;
	simulate_message_received();
	return true;
}

bool processQueryLocalPose(mobility::QueryLocalPose &query)
{
	std::cout << "Recv QueryLocalPose\n";

	mobility::ReportLocalPose *response = new mobility::ReportLocalPose();	
	response->setX_m(reportLocalPose->getX_m());
	response->setY_m(reportLocalPose->getY_m());
	response->setYaw_rad(reportLocalPose->getYaw_rad());
	
	response->setPresenceVector(65535);
	response->setDestination(query.getSource());
	component->sendMessage(response);
	return true;
}

bool processSetLocalPose(mobility::SetLocalPose &query)
{
	std::cout << "Recv SetLocalPose\n";
	//model::Message* message = &query;
	messages.push_back(new mobility::SetLocalPose(query));
	//messages.push_back(tr1::shared_ptr<model::Message*>(new mobility::SetLocalPose(query)));
	return true;
}

int init()
{
	//core::Base component;
	component = new core::Base();
	component->setName("igvc");

	component->addMessageCallback(processQueryServices);
	component->addMessageCallback(processQueryLocalPose);
	component->addMessageCallback(processSetLocalPose);

	//Init Messages
	reportLocalPose = new mobility::ReportLocalPose();
	reportLocalPose->setPresenceVector(65535);
	reportLocalPose->setX_m(15);
	reportLocalPose->setY_m(15);
	reportLocalPose->setYaw_rad(1.5);

	component->publish(reportLocalPose);
	
	component->run();

	return 0;
}

int message_received = 0;

int loop(int input)
{
	return messages.size();
}

int simulate_message_received()
{
	message_received += 1;
	return message_received;
}

void stop()
{
	//component->stop();
	delete component;
	delete &messages;
	delete reportLocalPose;

}

void updateLocalPose(double X_m, double Y_m, double Yaw_rad)
{
	reportLocalPose->setX_m(X_m);
	reportLocalPose->setY_m(Y_m);
	reportLocalPose->setYaw_rad(Yaw_rad);

	//reportLocalPose->setDestination(transport::Address(1,1,2));
	//component->sendMessage(reportLocalPose);

	/*core::QueryServices *query = new core::QueryServices();
	query->setDestination(transport::Address(1,1,2));
	component->sendMessage(query);*/
}

void readMessage(char* messageString)
{
	if(messages.size() == 0)
	{
		strcpy(messageString, "empty");
		return;
	}
	model::Message *m = messages.back();
	//model::Message *m = *messages.back().get();

	char response[200];
	
	switch(m->getId())
	{
	case mobility::SetLocalPose::ID:
		//double yaw = ((mobility::SetLocalPose*) *(messages.end() - 1))->getYaw_rad();
		
		mobility::SetLocalPose *message = (mobility::SetLocalPose*) m;
		sprintf(response, "SetLocalPose;%f;%f;%f", message->getX_m(), message->getY_m(), message->getYaw_rad());

		break;
	}

	strcpy(messageString, response);
	messages.pop_back();
	
	//return messages.size();
}

char* stringTest(char* input)
{
	char* string = input;
	//string[0] = 'v';
	char test[8] = "testing";
	strcpy(input, test);
	
	return test;
	//string test = input;
	//test.append(" added in c++");
	//return test.c_str();
}