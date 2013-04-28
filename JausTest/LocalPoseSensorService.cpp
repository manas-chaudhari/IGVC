#include "MyComponent.h"

ReportLocalPose MyComponent::getReportLocalPose(QueryLocalPose *queryLocalPose)
{
	reportLocalPose.setPresenceVector(queryLocalPose->getQueryPresenceVector());
	
	/*TODO:
		Timestamp
		Either request LabVIEW from here for latest values or update
		reportLocalPose through some loop function */

	return reportLocalPose;
}

bool MyComponent::updateLocalPose(SetLocalPose* setLocalPose)
{
	if (setLocalPose->isXEnabled())
	{
		this->reportLocalPose.setX_m(setLocalPose->getX_m());
	}

	if (setLocalPose->isYEnabled())
	{
		this->reportLocalPose.setY_m(setLocalPose->getY_m());
	}
	
	if (setLocalPose->isYawEnabled())
	{
		this->reportLocalPose.setYaw_rad(setLocalPose->getYaw_rad());
	}

	//TODO: Update these values for actual robot
	//Possibly make the loop function return values from various set messages

	return true;
}

bool MyComponent::isControllingLposClient(SetLocalPose *setLocalPose)
{
	return (setLocalPose->getSource() == this->controllerAddress);
}
