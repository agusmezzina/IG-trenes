#include "SimulationTimer.h"
#include <future>
#include <chrono>
#include <cmath>
#include <iostream>

SimulationTimer::SimulationTimer(std::queue<DataPacket>* rawData, World* worldData)
{
	this->rawData = rawData;
	this->worldData = worldData;
}

void SimulationTimer::run()
{
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		if (!rawData->empty())
		{
			auto data = rawData->front();
			worldData->updateEntityPosition(data.getID(), osg::Vec3f(data.getX(), data.getY(), data.getZ()));
			worldData->updateEntityVelocity(data.getID(), osg::Vec3f(data.getVx(), data.getVy(), data.getVz()));
			rawData->pop();
		}
	}
}

SimulationTimer::~SimulationTimer()
{
}
