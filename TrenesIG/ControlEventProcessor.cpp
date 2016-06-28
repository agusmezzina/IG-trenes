//#include "stdafx.h"
#include "ControlEventProcessor.h"
#include <iostream>

ControlEventProcessor::ControlEventProcessor()
{
}


ControlEventProcessor::~ControlEventProcessor()
{
}

void ControlEventProcessor::OnPacketReceived(CigiBasePacket *Packet)
{
	CigiIGCtrlV3_3 *InPckt = (CigiIGCtrlV3_3 *)Packet;
	SetOrigPckt(InPckt);

	/*std::cout << "===> IGCtrl <===" << std::endl;

	std::cout << "Version ==> " << InPckt->GetVersion() << std::endl;
	std::cout << "DatabaseID ==> " << InPckt->GetDatabaseID() << std::endl;
	std::cout << "IGMode ==> " << InPckt->GetIGMode() << std::endl;
	std::cout << "TimestampValid ==> " << InPckt->GetTimeStampValid() << std::endl;
	if (InPckt->GetSmoothingEn())
		std::cout << "Smoothing Enabled" << std::endl;
	else
		std::cout << "Smoothing Disabled" << std::endl;
	std::cout << "MinorVersion ==> " << InPckt->GetMinorVersion() << std::endl;
	std::cout << "FrameCntr ==> " << InPckt->GetFrameCntr() << std::endl;
	std::cout << "TimeStampV3 ==> " << InPckt->GetTimeStamp() << std::endl;
	std::cout << "IGFrameCntr ==> " << InPckt->GetLastRcvdIGFrame() << std::endl;*/
}