#pragma once
#include <CigiIGSession.h>
#include <memory>
#include "DataEventProcessor.h"
#include "ControlEventProcessor.h"
#include "RateEventProcessor.h"
#include "TrajectoryEventProcessor.h"
#include "World.h"

namespace osgCigi
{
	class CigiSessionHandler
	{
	public:
		CigiSessionHandler(World* data, int bufferSize);
		void processIncomingMessage(unsigned char* buffer, size_t bufferSize);
		virtual ~CigiSessionHandler();
	private:
		std::unique_ptr<CigiIGSession> cigiSession;
		CigiIncomingMsg* inMsg;
		std::unique_ptr<DataEventProcessor> dataProcessor;
		std::unique_ptr<ControlEventProcessor> controlProcessor;
		std::unique_ptr<RateEventProcessor> rateProcessor;
		std::unique_ptr<TrajectoryEventProcessor> trajectoryProcessor;
	};
}

