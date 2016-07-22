#include "stdafx.h"
#include "RateEventProcessor.h"


RateEventProcessor::RateEventProcessor(World* data) : TPckt(nullptr), data(data)
{
}

void RateEventProcessor::OnPacketReceived(CigiBasePacket *Packet)
{
	CigiRateCtrlV3_2 *InPckt = (CigiRateCtrlV3_2 *)Packet;
	setOriginPacket(InPckt);

	data->updateEntityVelocity(InPckt->GetEntityID(), osg::Vec3f(InPckt->GetXRate(), InPckt->GetYRate(), InPckt->GetZRate()));

}

RateEventProcessor::~RateEventProcessor()
{
}
