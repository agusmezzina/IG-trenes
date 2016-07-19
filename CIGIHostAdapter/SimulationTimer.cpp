#include "SimulationTimer.h"
#include <future>
#include <chrono>
#include <iostream>

SimulationTimer::SimulationTimer(std::queue<DataPacket>* rawData, World* worldData)
{
	this->rawData = rawData;
	this->worldData = worldData;
}

void SimulationTimer::run()
{
	int count = 0;
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		count++;
		if (!rawData->empty())
		{
			auto data = rawData->front();
			//std::cout << data.getY() << std::endl;
			if ((count % 40) == 0)
				worldData->updateEntityState(data.getID(), data.getX(), data.getY(), data.getZ(), 0, 0, 0);
			rawData->pop();
		}
	}
}

SimulationTimer::~SimulationTimer()
{
}
