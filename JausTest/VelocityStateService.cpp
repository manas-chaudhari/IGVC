#include "stdafx.h"
#include "MyComponent.h"

ReportVelocityState MyComponent::getReportVelocityState(QueryVelocityState *queryVelocityState)
{
	reportVelocityState.setPresenceVector(queryVelocityState->getQueryPresenceVector());
	//TODO: check if values are present
	return reportVelocityState;
}