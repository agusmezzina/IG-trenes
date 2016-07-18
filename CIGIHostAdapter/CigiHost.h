#pragma once
#include <CigiHostSession.h>
#include <CigiIGCtrlV3_2.h>
#include <CigiEntityCtrlV3_3.h>
#include <memory>
#include "IGControlProcessor.h"
#include "SceneData.h"
#include "World.h"

class CigiHost
{
public:
	CigiHost(World* data);
	void run();
	virtual ~CigiHost();
private:
	std::unique_ptr<CigiHostSession> cigiSession;
	std::unique_ptr<IGControlProcessor> ctrlProcessor;
	CigiOutgoingMsg* outMsg;
	CigiIncomingMsg* inMsg;
	CigiIGCtrlV3_2 igControl;
	CigiEntityCtrlV3_3 ownship;
	World* data;
};

