#pragma once
#include "CigiBaseEventProcessor.h"
#include "CigiRateCtrlV3_2.h"
#include "World.h"

class RateEventProcessor : public CigiBaseEventProcessor
{
public:
	RateEventProcessor(World* data);
	virtual ~RateEventProcessor();
	virtual void OnPacketReceived(CigiBasePacket *Packet);
	void setOriginPacket(CigiRateCtrlV3_2 *TPcktIn) { TPckt = TPcktIn; }

protected:
	CigiRateCtrlV3_2 *TPckt;
	World* data;
};

