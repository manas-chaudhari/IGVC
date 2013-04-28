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
#include "..\JausTest\MyComponent.h"

using namespace openjaus;
using namespace std;

class IGVC_Component : public MyComponent
{
public:
	vector<model::Message*> messages;

	IGVC_Component():
		MyComponent()
	{
	}

	bool updateLocalPose(SetLocalPose* setLocalPose)
	{
		this->messages.push_back(new mobility::SetLocalPose(setLocalPose));
		cout << "Added message to list\n";
		return MyComponent::updateLocalPose(setLocalPose);
	}
};

IGVC_Component *component;

int init()
{
	//core::Base component;
	component = new IGVC_Component();
	
	component->run();

	return 0;
}

int message_received = 0;

int loop(int input)
{
	return component->messages.size();
}

void stop()
{
	//component->stop();
	delete component;
	//delete &messages;

}

void updateLocalPose(double X_m, double Y_m, double Yaw_rad)
{
	component->reportLocalPose.setX_m(X_m);
	component->reportLocalPose.setY_m(Y_m);
	component->reportLocalPose.setYaw_rad(Yaw_rad);

	//reportLocalPose->setDestination(transport::Address(1,1,2));
	//component->sendMessage(reportLocalPose);

	/*core::QueryServices *query = new core::QueryServices();
	query->setDestination(transport::Address(1,1,2));
	component->sendMessage(query);*/
}

void updateVelocityState(double VelocityX_mps, double YawRate_rps)
{
	component->reportVelocityState.setVelocityX_mps(VelocityX_mps);
	component->reportVelocityState.setYawRate_rps(YawRate_rps);
}

void readMessage(char* messageString)
{
	if(component->messages.size() == 0)
	{
		strcpy(messageString, "empty");
		return;
	}
	model::Message *m = component->messages.back();
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
	component->messages.pop_back();
	
	//return messages.size();
}
