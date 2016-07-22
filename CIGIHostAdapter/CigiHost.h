#pragma once
#include <CigiHostSession.h>
#include <CigiIGCtrlV3_2.h>
#include <CigiEntityCtrlV3_3.h>
#include <CigiRateCtrlV3_2.h>
#include <memory>
#include <queue>
#include "IGControlProcessor.h"
#include "DataPacket.h"
#include "SceneData.h"
#include "World.h"

class CigiHost
{
public:
	CigiHost(World* data, World* ghost, std::queue<DataPacket>* rawData);
	void run();
	virtual ~CigiHost();
private:
	std::unique_ptr<CigiHostSession> cigiSession;
	std::unique_ptr<IGControlProcessor> ctrlProcessor;
	CigiOutgoingMsg* outMsg;
	CigiIncomingMsg* inMsg;
	CigiIGCtrlV3_2 igControl;
	CigiEntityCtrlV3_3 ownship;
	CigiRateCtrlV3_2 rateData;
	World* data;
	World* ghost;
	std::queue<DataPacket>* rawData;
};

