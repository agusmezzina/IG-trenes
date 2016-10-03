#pragma once

#include "CigiBaseEventProcessor.h"
#include "CigiEntityCtrlV3_3.h"
#include "World.h"
#include <osg\MatrixTransform>

namespace osgCigi
{
	class DataEventProcessor : public CigiBaseEventProcessor
	{
	public:
		DataEventProcessor(World* data);
		virtual ~DataEventProcessor();
		virtual void OnPacketReceived(CigiBasePacket *Packet);
		void setOriginPacket(CigiEntityCtrlV3_3 *TPcktIn) { TPckt = TPcktIn; }

	protected:
		CigiEntityCtrlV3_3 *TPckt;
		World* data;
	};
}