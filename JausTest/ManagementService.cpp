#include "stdafx.h"
#include "MyComponent.h"


bool MyComponent::storeID(SetEmergency *setEmergency)
{
	// Start of user code for action storeID(SetEmergency *setEmergency):
	storedIds.push_back(setEmergency->getSource());
	return true;
	// End of user code
}

bool MyComponent::deleteID(ClearEmergency *clearEmergency)
{
	// Start of user code for action deleteID(ClearEmergency *clearEmergency):
	for(size_t i = 0; i < storedIds.size(); i++)
	{
		if(storedIds[i] == clearEmergency->getSource())
		{
			storedIds.erase(storedIds.begin() + i);
			return true;
		}
	}
	return false;
	// End of user code
}

core::ReportStatus MyComponent::getReportStatus(QueryStatus *queryStatus)
{
	// Start of user code for action getReportStatus(QueryStatus *queryStatus):
	core::ReportStatus message;
	model::State *state = accessStateMachine.getCurrentState();

	if(state == &init)
	{
		message.setStatus(core::StatusEnumeration::INIT);
	}
	else if(state == &shutdown)
	{
		message.setStatus(core::StatusEnumeration::SHUTDOWN);
	}
	else if(state == &notControlled)
	{
		message.setStatus(core::StatusEnumeration::STANDBY);
	}
	else if(state == &emergency)
	{
		message.setStatus(core::StatusEnumeration::EMERGENCY);
	}
	else if(state == &controlled)
	{
		model::State *state = standbyReady.getCurrentState();
		if(state == &standby)
		{
			message.setStatus(core::StatusEnumeration::STANDBY);
		}
		else if(state == &ready)
		{
			message.setStatus(core::StatusEnumeration::READY);
		}
	}

	return message;
	// End of user code
}


bool MyComponent::isControllingClient(Reset *reset)
{
	// Start of user code for action isControllingClient(Reset *reset):
	if(controllerAddress == reset->getSource())
	{
		return true;
	}
	return false;

	// End of user code
}

bool MyComponent::isControllingClient(Shutdown *shutdown)
{
	// Start of user code for action isControllingClient(Shutdown *shutdown):
	if(controllerAddress == shutdown->getSource())
	{
		return true;
	}
	return false;
	// End of user code
}


bool MyComponent::isIDStored(ClearEmergency *clearEmergency)
{
	// Start of user code for action isIDStored(ClearEmergency *clearEmergency):
	for(size_t i = 0; i < storedIds.size(); i++)
	{
		if(storedIds[i] == clearEmergency->getSource())
		{
			return true;
		}
	}
	return false;

	// End of user code
}

void MyComponent::initialized()
{
	LOG_DEBUG("Managed component initialized");
	Initialized *initEvent = new Initialized();
	receiveThread.push(initEvent);
}