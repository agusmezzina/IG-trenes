#pragma once
#include "SceneData.h"
#include "DataPacket.h"
#include "World.h"
#include <queue>

class SimulationTimer
{
public:
	SimulationTimer(std::queue<DataPacket>* rawData, World* worldData);
	void run();
	virtual ~SimulationTimer();
private:
	std::queue<DataPacket>* rawData;
	World* worldData;
};

