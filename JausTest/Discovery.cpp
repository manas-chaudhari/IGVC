#include "stdafx.h"
#include "MyComponent.h"

bool MyComponent::processReportIdentification(core::ReportIdentification &reportIdentification)
{
	log("Received ReportIdentification");
	if(reportIdentification.getSource() == COP_Address)
	{
		copDiscovered = true;
		log("Discovered COP");
		pingTimer->stop();
	}
	return true;
}

void MyComponent::setupDiscovery()
{
	receive.addMessageCallback(&MyComponent::processReportIdentification, this);
}

void MyComponent::startDiscovery()
{
	copDiscovered = false;
	
	pingTimer = new openjaus::system::Timer(TIMER_METHOD(MyComponent, sendQueryIdentification), this);
	pingTimer->setInterval(5000);

}

void MyComponent::sendQueryIdentification(openjaus::system::Timer *timer)
{
	core::QueryIdentification *queryIdentification = new core::QueryIdentification();
	queryIdentification->setDestination(COP_Address);
	sendMessage(queryIdentification);
	
	log("Sent query identification to COP ..");
}