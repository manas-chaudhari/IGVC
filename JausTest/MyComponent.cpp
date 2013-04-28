#include "MyComponent.h"


MyComponent::MyComponent(void) : Base(),
	localPoseControlledLoop(),
	localPoseDefaultLoop(),
	reportLocalPose()
{
	name = "igvc";
	addLocalPoseService();
}

void MyComponent::addLocalPoseService()
{
	
	model::Service *localPoseSensorService = new model::Service();
	localPoseSensorService->setName("LocalPoseSensor");
	localPoseSensorService->setUri("urn:jaus:jss:mobility:LocalPoseSensor");
	localPoseSensorService->setVersionMajor(1);
	localPoseSensorService->setVersionMinor(0);
	this->implements->push_back(localPoseSensorService);

	localPoseControlledLoop.setInterface(this);
	localPoseControlledLoop.setTransportInterface(this);
	controlled.addTransition(localPoseControlledLoop);

	localPoseDefaultLoop.setInterface(this);
	localPoseDefaultLoop.setTransportInterface(this);
	accessStateMachine.addDefaultStateTransition(localPoseDefaultLoop);

	this->reportLocalPose.setX_m(34.5);
	this->reportLocalPose.setY_m(20);
	this->reportLocalPose.setYaw_rad(0.15);

	publish(&reportLocalPose);
}


MyComponent::~MyComponent(void)
{
}
