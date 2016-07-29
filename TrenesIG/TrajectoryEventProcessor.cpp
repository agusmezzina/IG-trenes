#include "stdafx.h"
#include "TrajectoryEventProcessor.h"


TrajectoryEventProcessor::TrajectoryEventProcessor(World* data) : TPckt(nullptr), data(data)
{
}


TrajectoryEventProcessor::~TrajectoryEventProcessor()
{
}

void TrajectoryEventProcessor::OnPacketReceived(CigiBasePacket *Packet)
{
	CigiTrajectoryDefV3 *InPckt = (CigiTrajectoryDefV3 *)Packet;
	setOriginPacket(InPckt);

	data->updateEntityAcceleration(InPckt->GetEntityID(), osg::Vec3f(InPckt->GetAccelX(), InPckt->GetAccelY(), InPckt->GetAccelZ()));

}
