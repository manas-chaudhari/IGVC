#include "stdafx.h"
#include "openjaus\core\Base.h"
#include "openjaus.h"
#include "openjaus/mobility.h"
#include "openjaus\mobility\LocalPoseSensor.h"
#include "openjaus\mobility\VelocityStateSensor.h"

using namespace openjaus;
using namespace mobility;

class MyComponent :
	public core::Base, 
	public mobility::LocalPoseSensorInterface,
	public mobility::VelocityStateSensorInterface
{
public:
	bool copDiscovered;
	transport::Address COP_Address;
	openjaus::system::Timer *pingTimer;
	void log(const char *fmt, ...); 

	// Report Messages
	ReportLocalPose reportLocalPose;
	ReportVelocityState reportVelocityState;
	// Methods

	MyComponent();
	MyComponent(uint16_t subsystem, unsigned char node, unsigned char component);
	virtual ~MyComponent(void);

	/// \brief Send action for ReportLocalPose with input message QueryLocalPose.
	/// Send action for ReportLocalPose with input message QueryLocalPose.
	/// \param[in] queryLocalPose - Input Trigger.
	/// \return ReportLocalPose Output Message.
	ReportLocalPose getReportLocalPose(QueryLocalPose *queryLocalPose);

	/// \brief UpdateLocalPose action with input SetLocalPose.
	/// UpdateLocalPose action with input SetLocalPose.
	/// \param[in]  setLocalPose - Input Trigger.
	/// \return Whether the message was properly processed by this action. 
    virtual bool updateLocalPose(SetLocalPose *setLocalPose);


	/// \brief isControllingLposClient condition.
	/// isControllingLposClient condition.
	/// \param[in]  setLocalPose - Input Trigger.
	/// \return Whether the condition is true. 
	bool isControllingLposClient(SetLocalPose *setLocalPose);

	ReportVelocityState getReportVelocityState(QueryVelocityState *queryVelocityState);

	void run();
	
	bool processReportIdentification(core::ReportIdentification &reportIdentification);	

protected:
	LocalPoseDefaultLoop localPoseDefaultLoop;
	LocalPoseControlledLoop localPoseControlledLoop;

	VelocityStateDefaultLoop velocityStateDefaultLoop;

private:
	void sendQueryIdentification(openjaus::system::Timer *timer);
	void addLocalPoseService();
	void addVelocityStateService();
	void setupDiscovery();
	void startDiscovery();

};

