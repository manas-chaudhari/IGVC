#include "stdafx.h"
#include "openjaus\core\Base.h"
#include "openjaus.h"
#include "openjaus/mobility.h"
#include "openjaus\mobility\LocalPoseSensor.h"
#include "openjaus\mobility\VelocityStateSensor.h"
#include "openjaus\core\Managed.h"

using namespace openjaus;
using namespace mobility;
using namespace core;

class MyComponent :
	public core::Base, 
	public mobility::LocalPoseSensorInterface,
	public mobility::VelocityStateSensorInterface,
	public core::ManagementInterface
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

	/// \brief StoreID action with input SetEmergency.
	/// StoreID action with input SetEmergency.
	/// \param[in]  setEmergency - Input Trigger.
	/// \return Whether the message was properly processed by this action. 
    virtual bool storeID(SetEmergency *setEmergency);

	/// \brief DeleteID action with input ClearEmergency.
	/// DeleteID action with input ClearEmergency.
	/// \param[in]  clearEmergency - Input Trigger.
	/// \return Whether the message was properly processed by this action. 
    virtual bool deleteID(ClearEmergency *clearEmergency);

	/// \brief Send action for ReportStatus with input message QueryStatus.
	/// Send action for ReportStatus with input message QueryStatus.
	/// \param[in] queryStatus - Input Trigger.
	/// \return ReportStatus Output Message.
	virtual ReportStatus getReportStatus(QueryStatus *queryStatus);


	/// \brief True if the message that triggered the transition is received from the client that is in control of this service
	/// True if the message that triggered the transition is received from the client that is in control of this service
	/// \param[in]  reset - Input Trigger.
	/// \return Whether the condition is true. 
	virtual bool isControllingClient(Reset *reset);

	/// \brief True if the message that triggered the transition is received from the client that is in control of this service
	/// True if the message that triggered the transition is received from the client that is in control of this service
	/// \param[in]  shutdown - Input Trigger.
	/// \return Whether the condition is true. 
	virtual bool isControllingClient(Shutdown *shutdown);


	/// \brief isIDStored condition.
	/// isIDStored condition.
	/// \param[in]  clearEmergency - Input Trigger.
	/// \return Whether the condition is true. 
	virtual bool isIDStored(ClearEmergency *clearEmergency);


	// Start of user code for additional methods:
	void initialized();

	void run();
	bool processReportIdentification(core::ReportIdentification &reportIdentification);	

protected:
	// LocalPoseSensor
	LocalPoseDefaultLoop localPoseDefaultLoop;
	LocalPoseControlledLoop localPoseControlledLoop;

	// VelocityStateSensor
	VelocityStateDefaultLoop velocityStateDefaultLoop;

	// Management
	ToReady toReady;
	Pause pause;
	ResetTransition resetTransition;
	ShutdownTransition shutdownTransition;
	PushToEmergency pushToEmergency;
	PopFromEmergency popFromEmergency;
	ManagementLoopback managementLoopback;
	InitializedTransition initializedTransition;
	model::State standby;
	model::State ready;
	model::State init;
	model::State shutdown;
	model::State emergency;

	model::StateMachine standbyReady;

	// Start of user code for additional members:
	std::vector< transport::Address > storedIds;


private:
	void sendQueryIdentification(openjaus::system::Timer *timer);
	void addLocalPoseService();
	void addVelocityStateService();
	void addManagementService();
	void setupDiscovery();
	void startDiscovery();

};

