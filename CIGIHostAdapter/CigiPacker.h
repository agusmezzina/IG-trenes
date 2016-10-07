#pragma once
#include <CigiHostSession.h>
#include <CigiIGCtrlV3_2.h>
#include <CigiEntityCtrlV3_3.h>
#include <CigiRateCtrlV3_2.h>
#include <CigiTrajectoryDefV3.h>
#include <memory>
#include "IGControlProcessor.h"
#include "Entity.h"

class CigiPacker
{
public:
	CigiPacker();
	virtual ~CigiPacker();
	void packData(const Entity& entity, float simulationTime, unsigned char** buffer, int& bufferSize);
	void freePacket();
private:
	std::unique_ptr<CigiHostSession> cigiSession;
	CigiOutgoingMsg* outMsg;
	CigiIncomingMsg* inMsg;
	/*CigiIGCtrlV3_2 igControl;
	CigiEntityCtrlV3_3 entityData;
	CigiRateCtrlV3_2 rateData;
	CigiTrajectoryDefV3 trajectoryData;*/
};

