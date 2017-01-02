//#include "stdafx.h"
#include "DataEventProcessor.h"
#include <iostream>

using namespace osgCigi;

DataEventProcessor::DataEventProcessor(World* data) : TPckt(nullptr), data(data)
{
}

void DataEventProcessor::OnPacketReceived(CigiBasePacket *Packet)
{
	CigiEntityCtrlV3_3 *InPckt = (CigiEntityCtrlV3_3 *)Packet;
	setOriginPacket(InPckt);

	data->updateEntityPosition(InPckt->GetEntityID(), osg::Vec3f(InPckt->GetXoff(), InPckt->GetYoff(), InPckt->GetZoff()));
	//data->updateEntityOrientation(InPckt->GetEntityID(), osg::Vec3f(InPckt->GetYaw(), InPckt->GetRoll(), InPckt->GetPitch()));
}

DataEventProcessor::~DataEventProcessor()
{
}
