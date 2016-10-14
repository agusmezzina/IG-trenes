#include "CigiPacker.h"


CigiPacker::CigiPacker()
{
	cigiSession = std::make_unique<CigiHostSession>(1, 32768, 2, 32768);
	CigiOutgoingMsg &Omsg = cigiSession->GetOutgoingMsgMgr();
	CigiIncomingMsg &Imsg = cigiSession->GetIncomingMsgMgr();
	outMsg = &Omsg;
	inMsg = &Imsg;
	cigiSession->SetCigiVersion(3, 3);
	cigiSession->SetSynchronous(false);
	inMsg->SetReaderCigiVersion(3, 3);
	inMsg->UsingIteration(false);
}

void CigiPacker::packData(const Entity& entity, float simulationTime, unsigned char** buffer, int& bufferSize)
{
	outMsg->BeginMsg();

	CigiIGCtrlV3_2 igControl;
	CigiEntityCtrlV3_3 entityData;
	CigiRateCtrlV3_2 rateData;
	CigiTrajectoryDefV3 trajectoryData;

	auto p = entity.getPosition();
	auto v = entity.getVelocity();
	auto a = entity.getAcceleration();

	igControl.SetIGMode(CigiBaseIGCtrl::Operate);
	igControl.SetTimeStampValid(true);
	igControl.SetTimeStamp(simulationTime);
	entityData.SetEntityID(1);
	entityData.SetEntityType(0);
	entityData.SetEntityState(CigiBaseEntityCtrl::Active);
	rateData.SetEntityID(1);
	trajectoryData.SetEntityID(1);

	entityData.SetLat(p.x());
	entityData.SetLon(p.y());
	entityData.SetAlt(p.z());
	rateData.SetXRate(v.x());
	rateData.SetYRate(v.y());
	rateData.SetZRate(v.z());
	trajectoryData.SetAccelX(a.x());
	trajectoryData.SetAccelY(a.y());
	trajectoryData.SetAccelZ(a.z());

	*outMsg << igControl;
	*outMsg << entityData;
	*outMsg << rateData;
	*outMsg << trajectoryData;
	outMsg->PackageMsg(buffer, bufferSize);
}

void CigiPacker::freePacket()
{
	outMsg->FreeMsg();
}

CigiPacker::~CigiPacker()
{
}
