#pragma once
#include "CigiBaseEventProcessor.h"
#include "CigiTrajectoryDefV3.h"
#include "World.h"

namespace osgCigi
{
	class TrajectoryEventProcessor : public CigiBaseEventProcessor
	{
	public:
		TrajectoryEventProcessor(World* data);
		virtual ~TrajectoryEventProcessor();
		virtual void OnPacketReceived(CigiBasePacket *Packet);
		void setOriginPacket(CigiTrajectoryDefV3 *TPcktIn) { TPckt = TPcktIn; }

	protected:
		CigiTrajectoryDefV3 *TPckt;
		World* data;
	};
}

