#include "stdafx.h"
#include "MyComponent.h"

bool test(openjaus::core::ReportIdentification &ReportIdentification)
{
	return true;
}

MyComponent::MyComponent(void) : Base(),
	localPoseControlledLoop(),
	localPoseDefaultLoop(),
	reportLocalPose(),
	velocityStateDefaultLoop(),
	reportVelocityState(),
	pingTimer(NULL),
	toReady(),
	pause(),
	resetTransition(),
	shutdownTransition(),
	pushToEmergency(),
	popFromEmergency(),
	managementLoopback(),
	initializedTransition(),
	standby(),
	ready(),
	init(),
	shutdown(),
	emergency(),
	standbyReady()
{
	name = "igvc";
	addLocalPoseService();
	addVelocityStateService();
}

MyComponent::MyComponent(uint16_t subsystem, unsigned char node, unsigned char component) : Base(),
	localPoseControlledLoop(),
	localPoseDefaultLoop(),
	reportLocalPose(),
	velocityStateDefaultLoop(),
	reportVelocityState(),
	pingTimer(NULL),
	toReady(),
	pause(),
	resetTransition(),
	shutdownTransition(),
	pushToEmergency(),
	popFromEmergency(),
	managementLoopback(),
	initializedTransition(),
	standby(),
	ready(),
	init(),
	shutdown(),
	emergency(),
	standbyReady()
{
	name = "igvc";
	COP_Address = transport::Address(subsystem, node, component);
	
	addLocalPoseService();
	addVelocityStateService();
	addManagementService();
	setupDiscovery();
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

void MyComponent::addManagementService()
{
	model::Service *managementService = new model::Service();
	managementService->setName("Management");
	managementService->setUri("urn:jaus:jss:core:Management");
	managementService->setVersionMajor(1);
	managementService->setVersionMinor(1);
	this->implements->push_back(managementService);
	
	standby.setName("Standby");
	ready.setName("Ready");
	
	init.setName("Init");
	accessStateMachine.addState(init);
	accessStateMachine.setStartingState(&init);
	shutdown.setName("Shutdown");
	accessStateMachine.addState(shutdown);
	emergency.setName("Emergency");
	accessStateMachine.addState(emergency);

	toReady.setInterface(this);
	toReady.setTransportInterface(this);
	toReady.setEndState(&ready);
	toReady.setStartState(&standby);
	standby.addTransition(toReady);
	
	pause.setInterface(this);
	pause.setTransportInterface(this);
	pause.setEndState(&standby);
	pause.setStartState(&ready);
	ready.addTransition(pause);
	
	resetTransition.setInterface(this);
	resetTransition.setTransportInterface(this);
	resetTransition.setEndState(&init);
	resetTransition.setStartState(&controlled);
	controlled.addTransition(resetTransition);
	
	shutdownTransition.setInterface(this);
	shutdownTransition.setTransportInterface(this);
	shutdownTransition.setEndState(&shutdown);
	controlled.addTransition(shutdownTransition);
	
	pushToEmergency.setInterface(this);
	pushToEmergency.setTransportInterface(this);
	pushToEmergency.setEndState(&emergency);
	accessStateMachine.addDefaultStateTransition(pushToEmergency);
	
	popFromEmergency.setInterface(this);
	popFromEmergency.setTransportInterface(this);
	popFromEmergency.setStartState(&emergency);
	emergency.addTransition(popFromEmergency);
	
	managementLoopback.setInterface(this);
	managementLoopback.setTransportInterface(this);
	accessStateMachine.addDefaultStateTransition(managementLoopback);
	
	initializedTransition.setInterface(this);
	initializedTransition.setTransportInterface(this);
	initializedTransition.setEndState(&notControlled);
	initializedTransition.setStartState(&init);
	init.addTransition(initializedTransition);
	
	standbyReady.setName("StandbyReady");
	standbyReady.addState(standby);
	standbyReady.addState(ready);
	standbyReady.setStartingState(&standby);
	controlled.addStateMachine(standbyReady);
	
    
    
	// Start of user code for Constructor:
	storedIds.clear();
	// End of user code
}

MyComponent::~MyComponent(void)
{
	if(pingTimer)
	{
		delete pingTimer;
	}
}

void MyComponent::run()
{
	Base::run();
	startDiscovery();
}

void MyComponent::log(const char *fmt, ...)
{
	va_list arg;
	va_start(arg, fmt);
	printf(fmt, arg);
	va_end(arg);
	printf("\n");
}