#include "CigiManager.h"


CigiManager::CigiManager()
{
	cigiSession = std::make_unique<CigiHostSession>(1, 32768, 2, 32768);
	ctrlProcessor = std::make_unique<IGControlProcessor>();
	CigiOutgoingMsg &Omsg = cigiSession->GetOutgoingMsgMgr();
	CigiIncomingMsg &Imsg = cigiSession->GetIncomingMsgMgr();
	outMsg = &Omsg;
	inMsg = &Imsg;
	cigiSession->SetCigiVersion(3, 3);
	cigiSession->SetSynchronous(false);
	inMsg->SetReaderCigiVersion(3, 3);
	inMsg->UsingIteration(false);
	// register SOF
	Imsg.RegisterEventProcessor(CIGI_SOF_PACKET_ID_V3_2, ctrlProcessor.get());	
}

void CigiManager::packData(const Entity& entity, unsigned char** buffer, int& bufferSize)
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
	entityData.SetEntityID(1);
	entityData.SetEntityType(0);
	entityData.SetEntityState(CigiBaseEntityCtrl::Active);
	rateData.SetEntityID(1);
	trajectoryData.SetEntityID(1);

	entityData.SetLat(p.x());
	entityData.SetLon(p.y());
	rateData.SetXRate(v.x());
	rateData.SetYRate(v.y());
	trajectoryData.SetAccelX(a.x());
	trajectoryData.SetAccelY(a.y());

	*outMsg << igControl;
	*outMsg << entityData;
	*outMsg << rateData;
	*outMsg << trajectoryData;
	outMsg->PackageMsg(buffer, bufferSize);
}

void CigiManager::freeMessage()
{
	outMsg->FreeMsg();
}

CigiManager::~CigiManager()
{
}
