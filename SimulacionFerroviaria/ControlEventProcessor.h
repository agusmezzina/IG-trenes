#pragma once
#include "CigiBaseEventProcessor.h"
#include "CigiIGCtrlV3_3.h"

class ControlEventProcessor : public CigiBaseEventProcessor
{
public:
	ControlEventProcessor();
	virtual ~ControlEventProcessor();
	virtual void OnPacketReceived(CigiBasePacket *Packet);
	void SetOrigPckt(CigiIGCtrlV3_3 *TPcktIn) { TPckt = TPcktIn; }
protected:
	CigiIGCtrlV3_3 *TPckt;
};

