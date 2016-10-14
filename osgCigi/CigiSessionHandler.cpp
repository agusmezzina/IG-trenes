#include "CigiSessionHandler.h"

using namespace osgCigi;

CigiSessionHandler::CigiSessionHandler(World* data, int bufferSize)
{
	cigiSession = std::make_unique<CigiIGSession>(1, bufferSize, 2, bufferSize);
	dataProcessor = std::make_unique<DataEventProcessor>(data);
	controlProcessor = std::make_unique<ControlEventProcessor>(data);
	rateProcessor = std::make_unique<RateEventProcessor>(data);
	trajectoryProcessor = std::make_unique<TrajectoryEventProcessor>(data);
	CigiIncomingMsg &Imsg = cigiSession->GetIncomingMsgMgr();
	inMsg = &Imsg;

	cigiSession->SetCigiVersion(3, 3);
	cigiSession->SetSynchronous(false);

	inMsg->SetReaderCigiVersion(3, 3);
	inMsg->UsingIteration(false);
	registerEventHandlers();
	
}

void CigiSessionHandler::processIncomingMessage(unsigned char* buffer, size_t bufferSize)
{
	inMsg->ProcessIncomingMsg(buffer, bufferSize);
}

void CigiSessionHandler::registerEventHandlers()
{
	inMsg->RegisterEventProcessor(CIGI_IG_CTRL_PACKET_ID_V3_2, controlProcessor.get());
	inMsg->RegisterEventProcessor(CIGI_ENTITY_CTRL_PACKET_ID_V3_3, dataProcessor.get());
	inMsg->RegisterEventProcessor(CIGI_RATE_CTRL_PACKET_ID_V3_2, rateProcessor.get());
	inMsg->RegisterEventProcessor(CIGI_TRAJECTORY_DEF_PACKET_ID_V3, trajectoryProcessor.get());
}

CigiSessionHandler::~CigiSessionHandler()
{
}
