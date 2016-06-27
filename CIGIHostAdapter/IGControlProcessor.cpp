#include "IGControlProcessor.h"
#include <iostream>

IGControlProcessor::IGControlProcessor()
{
}

void IGControlProcessor::OnPacketReceived(CigiBasePacket *Packet)
{
	CigiIGCtrlV3_2 *InPckt = (CigiIGCtrlV3_2 *)Packet;
	setOriginPacket(InPckt);

	// We are actually not processing any information in the SOF
	std::cout << "Recibido" << std::endl;
}


IGControlProcessor::~IGControlProcessor()
{
}
