#pragma once
#include "CigiBaseEventProcessor.h"
#include "CigiIGCtrlV3_3.h"
#include "World.h"

namespace osgCigi
{
	class ControlEventProcessor : public CigiBaseEventProcessor
	{
	public:
		ControlEventProcessor(World*);
		virtual ~ControlEventProcessor();
		virtual void OnPacketReceived(CigiBasePacket *Packet);
		void SetOrigPckt(CigiIGCtrlV3_3 *TPcktIn) { TPckt = TPcktIn; }
	protected:
		CigiIGCtrlV3_3 *TPckt;
		World* data;
	};
}

