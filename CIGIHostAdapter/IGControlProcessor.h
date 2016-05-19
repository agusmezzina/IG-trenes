#pragma once
#include <CigiBaseEventProcessor.h>
#include <CigiIGCtrlV3_2.h>

class IGControlProcessor : public CigiBaseEventProcessor
{
public:
	IGControlProcessor();
	virtual ~IGControlProcessor();
	virtual void OnPacketReceived(CigiBasePacket *Packet);
	void setOriginPacket(CigiIGCtrlV3_2 *TPcktIn) { TPckt = TPcktIn; }

protected:
	CigiIGCtrlV3_2 *TPckt;
};

