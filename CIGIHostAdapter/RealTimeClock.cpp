#include <thread>
#include "RealTimeClock.h"

void RealTimeClock::sync(float simulationTime)
{
	auto deltaSimTime = simulationTime - prevSimulationTime;
	auto realTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> deltaRealTime = realTime - prevRealTime;
	int waitFor = (deltaSimTime - deltaRealTime.count() - compensationTime) * 1000;
	if (waitFor > 0)
		std::this_thread::sleep_for(std::chrono::milliseconds(waitFor));
	else
		waitFor = 0;

	prevRealTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> elapsedTime = prevRealTime - initial;
	compensationTime = elapsedTime.count() - simulationTime;
	prevSimulationTime = simulationTime;
}

void RealTimeClock::init()
{
	prevRealTime = std::chrono::high_resolution_clock::now();
	realTime = prevRealTime;
	initial = prevRealTime;
	compensationTime = 0;
	prevSimulationTime = 0;
}

RealTimeClock::RealTimeClock()
{
}


RealTimeClock::~RealTimeClock()
{
}
