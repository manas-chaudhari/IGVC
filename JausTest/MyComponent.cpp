#include "MyComponent.h"


MyComponent::MyComponent(void) : Base(),
	localPoseControlledLoop(),
	localPoseDefaultLoop(),
	reportLocalPose(),
	velocityStateDefaultLoop(),
	reportVelocityState()
{
	name = "igvc";
	addLocalPoseService();
	addVelocityStateService();
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

void MyComponent::addVelocityStateService()
{
	model::Service *velocityStateSensorService = new model::Service();
	velocityStateSensorService->setName("VelocityStateSensor");
	velocityStateSensorService->setUri("urn:jaus:jss:mobility:VelocityStateSensor");
	velocityStateSensorService->setVersionMajor(1);
	velocityStateSensorService->setVersionMinor(0);
	this->implements->push_back(velocityStateSensorService);
	
	velocityStateDefaultLoop.setInterface(this);
	velocityStateDefaultLoop.setTransportInterface(this);
	receive.addDefaultStateTransition(velocityStateDefaultLoop);
	
	this->reportVelocityState.setVelocityX_mps(1.5);
	this->reportVelocityState.setYawRate_rps(0.2);

	publish(&reportVelocityState);
}

MyComponent::~MyComponent(void)
{
}
