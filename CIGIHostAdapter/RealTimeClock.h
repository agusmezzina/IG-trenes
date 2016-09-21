#pragma once
#include <chrono>

class RealTimeClock
{
public:
	RealTimeClock();
	virtual ~RealTimeClock();
	void sync(float simulationTime);
	void init();
private:
	std::chrono::high_resolution_clock::time_point prevRealTime;
	std::chrono::high_resolution_clock::time_point realTime;
	std::chrono::high_resolution_clock::time_point initial;
	float compensationTime;
	float prevSimulationTime;
};

