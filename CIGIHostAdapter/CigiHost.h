#pragma once
#include "SimulationState.h"
#include "IGControlProcessor.h"
#include <boost\array.hpp>
#include <boost\asio.hpp>
#include <CigiHostSession.h>
#include <CigiIGCtrlV3_2.h>
#include <CigiEntityCtrlV3_3.h>
#include <memory>

class CigiHost
{
public:
	CigiHost(SimulationState* ss);
	void runCigi();
	virtual ~CigiHost();
private:
	SimulationState* state;
	std::unique_ptr<CigiHostSession> cigiSession;
	std::unique_ptr<IGControlProcessor> ctrlProcessor;
	CigiOutgoingMsg* outMsg;
	CigiIncomingMsg* inMsg;
	CigiIGCtrlV3_2 igControl;
	CigiEntityCtrlV3_3 ownship;
};

