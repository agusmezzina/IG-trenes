//#include "stdafx.h"
#include "DataEventProcessor.h"
#include <iostream>

DataEventProcessor::DataEventProcessor(World* data) : TPckt(nullptr), data(data)
{
}

void DataEventProcessor::OnPacketReceived(CigiBasePacket *Packet)
{
	CigiEntityCtrlV3_3 *InPckt = (CigiEntityCtrlV3_3 *)Packet;
	setOriginPacket(InPckt);

	data->updateEntityPosition(InPckt->GetEntityID(), osg::Vec3f(InPckt->GetXoff(), InPckt->GetYoff(), InPckt->GetZoff()));

	/*std::cout << "===>EntityCtrl <===" << std::endl;
	std::cout << "EntityID ==> " << InPckt->GetEntityID() << std::endl;
	std::cout << "EntityState ==> " << InPckt->GetEntityState() << std::endl;
	std::cout << "AttachState ==> " << InPckt->GetAttachState() << std::endl;
	std::cout << "CollisionDetectEn ==> " << InPckt->GetCollisionDetectEn() << std::endl;
	std::cout << "InheritAlpha ==> " << InPckt->GetInheritAlpha() << std::endl;
	std::cout << "GrndClamp ==> " << InPckt->GetGrndClamp() << std::endl;
	std::cout << "AnimationDir ==> " << InPckt->GetAnimationDir() << std::endl;
	std::cout << "AnimationLoopMode ==> " << InPckt->GetAnimationLoopMode() << std::endl;
	std::cout << "AnimationState ==> " << InPckt->GetAnimationState() << std::endl;
	if (InPckt->GetSmoothingEn())
		std::cout << "Smoothing Enabled" << std::endl;
	else
		std::cout << "Smoothing Disabled" << std::endl;
	std::cout << "Alpha ==> " << InPckt->GetAlpha() << std::endl;
	std::cout << "EntityType ==> " << InPckt->GetEntityType() << std::endl;
	std::cout << "ParentID ==> " << InPckt->GetParentID() << std::endl;
	std::cout << "Roll ==> " << InPckt->GetRoll() << std::endl;
	std::cout << "Pitch ==> " << InPckt->GetPitch() << std::endl;
	std::cout << "Yaw ==> " << InPckt->GetYaw() << std::endl;
	std::cout << "Xoff ==> " << InPckt->GetXoff() << std::endl;
	std::cout << "Yoff ==> " << InPckt->GetYoff() << std::endl;
	std::cout << "Zoff ==> " << InPckt->GetZoff() << std::endl;*/

}

DataEventProcessor::~DataEventProcessor()
{
}
